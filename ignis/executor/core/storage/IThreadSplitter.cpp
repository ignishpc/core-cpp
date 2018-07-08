
#include "IThreadSplitter.h"
#include "IMemoryObject.h"
#include <boost/lockfree/spsc_queue.hpp>


using namespace std;
using namespace ignis::executor::core;
using namespace ignis::executor::core::storage;
using boost::lockfree::spsc_queue;

pair<shared_ptr<ICoreReadIterator<IObject::Any>>, shared_ptr<ICoreWriteIterator<IObject::Any>>>
IThreadSplitter::operator[](size_t pos) {
    return iterators[pos];
};

class ISplitterIterator : public ICoreReadIterator<IObject::Any>, public ICoreWriteIterator<IObject::Any> {
public:
    ISplitterIterator(shared_ptr<ICoreReadIterator<IObject::Any>> it_read,
                      shared_ptr<ICoreWriteIterator<IObject::Any>> it_write,
                      shared_ptr<ignis::data::IManager<IObject::Any>> m,
                      std::mutex &mutex_read, std::mutex &mutex_write, size_t buffer)
            : mutex_read(mutex_read), mutex_write(mutex_write), m(m), it_read(it_read), it_write(it_write),
              buffer(buffer) {
        buffer_input = newBuffer(buffer);
        buffer_input_read = buffer_input->readIterator();
        buffer_output = newBuffer(buffer);
        buffer_output_write = buffer_output->writeIterator();
    }

    IObject::Any &next() override {
        elems--;
        return buffer_input_read->next();
    }

    bool hashNext() override {
        if (elems > 0) {
            return true;
        }
        {
            std::lock_guard<std::mutex> lock(mutex_write);
            flush(*buffer_output);
        }
        {
            std::lock_guard<std::mutex> lock(mutex_read);
            take();
        }
        return elems > 0;
    }

    bool isMoved() override {
        return true;
    }

    void write(IObject::Any &obj) override {
        buffer_output_write->write(obj);
    }

    void write(IObject::Any &&obj) override {
        buffer_output_write->write(obj);
    }

    void take() {
        buffer_input->clear();
        readToWrite<IObject::Any>(*it_read, *buffer_input->writeIterator(), buffer);
        elems = buffer;
        buffer_input_read = buffer_input->readIterator();
    }

    shared_ptr<IObject> newBuffer(size_t size) {
        return make_shared<IMemoryObject>(m, size);
    }

    void flush(IObject &obj) {
        auto reader = obj.readIterator();
        while (reader->hashNext()) {
            it_write->write((IObject::Any) reader->next());
        }
    }

    virtual ~ISplitterIterator() override {}

protected:
    std::mutex &mutex_read;
    std::mutex &mutex_write;
    shared_ptr<ICoreReadIterator<IObject::Any>> it_read;
    shared_ptr<ICoreWriteIterator<IObject::Any>> it_write;
    shared_ptr<ignis::data::IManager<IObject::Any>> m;
    size_t elems;
    size_t buffer;
    shared_ptr<IObject> buffer_input;
    shared_ptr<ICoreReadIterator<IObject::Any>> buffer_input_read;
    shared_ptr<IObject> buffer_output;
    shared_ptr<ICoreWriteIterator<IObject::Any>> buffer_output_write;
};


class IOrderSplitterIterator : public ISplitterIterator {
public:

    IOrderSplitterIterator(size_t id, size_t threads,
                           shared_ptr<ICoreReadIterator<IObject::Any>> it_read,
                           shared_ptr<ICoreWriteIterator<IObject::Any>> it_write,
                           shared_ptr<ignis::data::IManager<IObject::Any>> m,
                           std::mutex &mutex_read, std::mutex &mutex_write, size_t buffer, size_t &write_next,
                           shared_ptr<vector<boost::lockfree::spsc_queue<std::shared_ptr<IObject>>>> &queues)
            : id(id), ISplitterIterator(it_read, it_write,m, mutex_read, mutex_write, buffer), write_next(write_next),
              threads(threads), queues(queues) {}

    bool hashNext() override {
        if (elems > 0) {
            return true;
        }
        if (id != write_next) {
            while (!(*queues)[id].push(buffer_output)) {
                sleep(1);
            }
            buffer_output = newBuffer(buffer);
            buffer_output_write = buffer_output->writeIterator();
        } else {
            std::lock_guard<std::mutex> lock(mutex_write);
            flush(*buffer_output);
            buffer_output->clear();
            for (size_t i = id + 1; i != id;) {
                i = i % threads;
                shared_ptr<IObject> other;
                if ((*queues)[id].pop(other)) {
                    flush(*other);
                } else {
                    write_next = i;
                    break;
                }
            }
        }
        std::lock_guard<std::mutex> lock(mutex_read);
        take();
        return elems > 0;
    }

    virtual ~IOrderSplitterIterator() override {}

private:
    size_t &write_next;
    size_t threads;
    size_t id;
    shared_ptr<vector<boost::lockfree::spsc_queue<std::shared_ptr<IObject>>>> queues;
};


IThreadSplitter::IThreadSplitter(IObject &object_in, IObject &object_out, size_t threads, size_t buffer, bool order) :
        object_in(object_in),
        object_out(object_out) {
    auto it_read = object_in.readIterator();
    auto it_write = object_in.writeIterator();
    auto manager = object_in.getManager();
    if (order) {
        auto queues = make_shared<vector<boost::lockfree::spsc_queue<std::shared_ptr<IObject>>>>();
        for (size_t i = 0; i < threads; i++) {
            auto it = make_shared<IOrderSplitterIterator>(i, threads, it_read, it_write, manager, mutex_read,
                                                          mutex_write,
                                                          buffer, write_next, queues);
            iterators.push_back(pair<decltype(it), decltype(it)>(it, it));
        }
    } else {
        for (size_t i = 0; i < threads; i++) {
            auto it = make_shared<ISplitterIterator>(it_read, it_write, manager, mutex_read, mutex_write, buffer);
            iterators.push_back(pair<decltype(it), decltype(it)>(it, it));
        }
    }
}