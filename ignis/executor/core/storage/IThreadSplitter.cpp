
#include "IThreadSplitter.h"
#include "IMemoryObject.h"
#include <boost/lockfree/spsc_queue.hpp>


using namespace ignis::executor::core;
using namespace ignis::executor::core::storage;
using boost::lockfree::spsc_queue;

std::pair<std::shared_ptr<iterator::ICoreReadIterator<IObject::Any>>, std::shared_ptr<iterator::ICoreWriteIterator<IObject::Any>>>
IThreadSplitter::operator[](size_t pos) {
    return iterators[pos];
};

class ISplitterIterator : public iterator::ICoreReadIterator<IObject::Any>, public iterator::ICoreWriteIterator<IObject::Any> {
public:
    ISplitterIterator(std::shared_ptr<iterator::ICoreReadIterator<IObject::Any>> it_read,
                      std::shared_ptr<iterator::ICoreWriteIterator<IObject::Any>> it_write,
                      std::shared_ptr<ignis::executor::api::IManager<IObject::Any>> m_in,
                      std::shared_ptr<ignis::executor::api::IManager<IObject::Any>> m_out,
                      std::mutex &mutex_read, std::mutex &mutex_write, size_t buffer)
            : mutex_read(mutex_read), mutex_write(mutex_write), m_in(m_in), m_out(m_out), it_read(it_read),
              it_write(it_write), buffer(buffer) {
        buffer_input = newBuffer(buffer, m_in);
        buffer_input_read = buffer_input->readIterator();
        buffer_output = newBuffer(buffer, m_out);
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
        iterator::readToWrite<IObject::Any>(*it_read, *buffer_input->writeIterator(), buffer);
        elems = buffer;
        buffer_input_read = buffer_input->readIterator();
    }

    std::shared_ptr<IObject> newBuffer(size_t size, std::shared_ptr<ignis::executor::api::IManager<IObject::Any>> m) {
        return std::make_shared<IMemoryObject>(m, size);
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
    std::shared_ptr<iterator::ICoreReadIterator<IObject::Any>> it_read;
    std::shared_ptr<iterator::ICoreWriteIterator<IObject::Any>> it_write;
    std::shared_ptr<ignis::executor::api::IManager<IObject::Any>> m_in;
    std::shared_ptr<ignis::executor::api::IManager<IObject::Any>> m_out;
    size_t elems;
    size_t buffer;
    std::shared_ptr<IObject> buffer_input;
    std::shared_ptr<ICoreReadIterator<IObject::Any>> buffer_input_read;
    std::shared_ptr<IObject> buffer_output;
    std::shared_ptr<ICoreWriteIterator<IObject::Any>> buffer_output_write;
};


class IOrderSplitterIterator : public ISplitterIterator {
public:

    IOrderSplitterIterator(size_t id, size_t threads,
                           std::shared_ptr<iterator::ICoreReadIterator<IObject::Any>> it_read,
                           std::shared_ptr<iterator::ICoreWriteIterator<IObject::Any>> it_write,
                           std::shared_ptr<ignis::executor::api::IManager<IObject::Any>> m_in,
                           std::shared_ptr<ignis::executor::api::IManager<IObject::Any>> m_out,
                           std::mutex &mutex_read, std::mutex &mutex_write, size_t buffer, size_t &write_next,
                           std::shared_ptr<std::vector<boost::lockfree::spsc_queue<std::shared_ptr<IObject>>>> &queues)
            : id(id), ISplitterIterator(it_read, it_write, m_in, m_out, mutex_read, mutex_write, buffer),
              write_next(write_next), threads(threads), queues(queues) {}

    bool hashNext() override {
        if (elems > 0) {
            return true;
        }
        if (id != write_next) {
            while (!(*queues)[id].push(buffer_output)) {
                sleep(1);
            }
            buffer_output = newBuffer(buffer, m_out);
            buffer_output_write = buffer_output->writeIterator();
        } else {
            std::lock_guard<std::mutex> lock(mutex_write);
            flush(*buffer_output);
            buffer_output->clear();
            for (size_t i = id + 1; i != id;) {
                i = i % threads;
                std::shared_ptr<IObject> other;
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
    std::shared_ptr<std::vector<boost::lockfree::spsc_queue<std::shared_ptr<IObject>>>> queues;
};


IThreadSplitter::IThreadSplitter(IObject &object_in, IObject &object_out, size_t threads, size_t buffer, bool order) :
        object_in(object_in),
        object_out(object_out) {
    auto it_read = object_in.readIterator();
    auto it_write = object_in.writeIterator();
    auto m_in = object_in.getManager();
    auto m_out = object_out.getManager();
    if (order) {
        auto queues = std::make_shared<std::vector<boost::lockfree::spsc_queue<std::shared_ptr<IObject>>>>();
        for (size_t i = 0; i < threads; i++) {
            auto it = std::make_shared<IOrderSplitterIterator>(i, threads, it_read, it_write, m_in, m_out, mutex_read,
                                                          mutex_write,
                                                          buffer, write_next, queues);
            iterators.push_back(std::pair<decltype(it), decltype(it)>(it, it));
        }
    } else {
        for (size_t i = 0; i < threads; i++) {
            auto it = std::make_shared<ISplitterIterator>(it_read, it_write, m_in, m_out, mutex_read, mutex_write, buffer);
            iterators.push_back(std::pair<decltype(it), decltype(it)>(it, it));
        }
    }
}