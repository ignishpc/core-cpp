#include "ISortModule.h"
#include <unordered_map>
#include <algorithm>
#include "../ILog.h"
#include "../IMessage.h"
#include "../storage/iterator/IElementIterator.h"
#include "../../../exceptions/IInvalidArgument.h"
#include "../storage/IMemoryObject.h"


using namespace ignis::executor::core;
using namespace ignis::executor::api;
using namespace ignis::executor::core::storage;
using namespace ignis::executor::core::modules;
using ignis::rpc::IRemoteException;

ISortModule::ISortModule(std::shared_ptr<IExecutorData> &executor_data) : IgnisModule(executor_data) {}


class NarutalOrder : public function::IFunction2<IObject::Any, IObject::Any, bool> {

    std::shared_ptr<ignis::data::handle::IOperator<storage::IObject::Any>> o;
public:
    NarutalOrder(const std::shared_ptr<ignis::data::handle::IOperator<storage::IObject::Any>> &o) : o(o) {}

    virtual bool call(IObject::Any &t1, IObject::Any &t2, IContext &context) {
        return o->less(t1, t2);
    }
};

void ISortModule::localCustomSort(const ignis::rpc::ISource &sf,const bool ascending) {
    try {
        auto less = loadSource<function::IFunction2<IObject::Any, IObject::Any, bool>>(sf);
        mergeSort(*less, ascending);
    } catch (exceptions::IException &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack(ex.getStacktrace());
        throw iex;
    } catch (std::exception &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack("UNKNOWN");
        throw iex;
    }
}

void ISortModule::localSort(const bool ascending) {
    try {
        NarutalOrder less(getManager(*executor_data->loadObject())->_operator());
        mergeSort(less, ascending);
    } catch (exceptions::IException &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack(ex.getStacktrace());
        throw iex;
    } catch (std::exception &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack("UNKNOWN");
        throw iex;
    }
}

void ISortModule::mergeSort(function::IFunction2<IObject::Any, IObject::Any, bool> &less,const bool ascending) {
    IGNIS_LOG(info) << "ISortModule sorting";
    auto object_in = executor_data->loadObject();
    auto size = object_in->getSize();
    if (size == 0) {
        IGNIS_LOG(info) << "ISortModule no data to sort";
        return;
    }
    auto manager = getManager(*object_in);
    auto object_out = getIObject(manager);
    size_t threads = executor_data->getThreads();
    auto &context = executor_data->getContext();

    auto reader = object_in->readIterator();
    decltype(object_in) parts[threads];
    decltype(reader) readers[threads];
    auto div = size / threads;
    auto mod = size % threads;
    if (object_in->getType() == storage::IMemoryObject::TYPE || threads == 1) {
        for (int t = 0; t < threads; t++) {
            readers[t] = object_in->readIterator();
            readers[t]->skip(t * div + (t < mod ? t : mod));
        }
    } else {
        for (int t = 0; t < threads; t++) {
            parts[t] = getIObject(manager, div + 1);
            storage::iterator::readToWrite(*reader, *parts[t]->writeIterator(), div + (t < mod ? 1 : 0));
            readers[t] = parts[t]->readIterator();
        }
        object_in = parts[0];
        executor_data->deleteLoadObject();
    }
    //parallel merge
    std::vector<decltype(object_in)> merges;
    threads = std::min(threads, (size_t) size / 10);
    threads = std::max(threads, (size_t) 1);
    IGNIS_LOG(info) << "ISortModule sorting with " << threads << " threads";
    less.before(executor_data->getContext());
#pragma omp parallel num_threads(threads)
    {
#pragma omp for
        for (int t = 0; t < threads; t++) {
            auto size_thread = size / threads + (t < mod ? 1 : 0);
            auto reader_thread = readers[t];
            std::vector<decltype(object_in)> clears;
            std::vector<decltype(object_in)> stack;
            storage::iterator::IElementIterator aux1(manager);
            storage::iterator::IElementIterator aux2(manager);

            for (size_t i = 0; true;) {
                while (i < size_thread &&
                       (stack.size() < 2 || stack[stack.size() - 1]->getSize() != stack[stack.size() - 2]->getSize())) {
                    decltype(object_in) obj;
                    if (clears.empty()) {
                        obj = getIObject(manager, size_thread / (i + 1));
                    } else {
                        obj = clears.back();
                        clears.pop_back();
                    }
                    stack.push_back(obj);
                    obj->writeIterator()->write((IObject::Any &&) reader_thread->next());
                    i++;
                }
                if (stack[0]->getSize() == size_thread) {
                    break;
                }

                auto r1 = stack[stack.size() - 1]->readIterator();
                auto r2 = stack[stack.size() - 2]->readIterator();
                decltype(object_in) obj;
                if (clears.empty()) {
                    obj = getIObject(manager, stack[stack.size() - 1]->getSize() + stack[stack.size() - 2]->getSize());
                } else {
                    obj = clears.back();
                    clears.pop_back();
                }
                auto writer = obj->writeIterator();
                aux1.write(r1->next());
                aux2.write(r2->next());

                while (true) {
                    if (less.call(aux1.next(), aux2.next(), context) == ascending) {
                        writer->write((IObject::Any &&) aux1.next());
                        if (r1->hasNext()) {
                            aux1.write(r1->next());
                        } else {
                            writer->write((IObject::Any &&) aux2.next());
                            storage::iterator::readToWrite(*r2, *writer, true);
                            break;
                        }
                    } else {
                        writer->write((IObject::Any &&) aux2.next());
                        if (r2->hasNext()) {
                            aux2.write(r2->next());
                        } else {
                            writer->write((IObject::Any &&) aux1.next());
                            storage::iterator::readToWrite(*r1, *writer, true);
                            break;
                        }
                    }
                }

                stack.back()->clear();
                clears.push_back(stack.back());
                stack.pop_back();

                stack.back()->clear();
                clears.push_back(stack.back());
                stack.pop_back();

                stack.push_back(obj);
            }
#pragma omp critical
            {
                merges.push_back(stack[0]);
            }
        }

        //combine
#pragma omp master
        {
            int n = merges.size();
            while (n > 1) {
                for (int i = 1; i < n; i += 2) {
#pragma omp task
                    {
                        auto r1 = merges[i - 1]->readIterator();
                        auto r2 = merges[i]->readIterator();
                        auto obj = getIObject(manager, merges[i - 1]->getSize() + merges[i]->getSize());
                        storage::iterator::IElementIterator aux1(manager);
                        storage::iterator::IElementIterator aux2(manager);
                        auto writer = obj->writeIterator();
                        aux1.write(r1->next());
                        aux2.write(r2->next());

                        while (true) {
                            if (less.call(aux1.next(), aux2.next(), context) == ascending) {
                                writer->write((IObject::Any &&) aux1.next());
                                if (r1->hasNext()) {
                                    aux1.write(r1->next());
                                } else {
                                    writer->write((IObject::Any &&) aux2.next());
                                    storage::iterator::readToWrite(*r2, *writer, true);
                                    break;
                                }
                            } else {
                                writer->write((IObject::Any &&) aux2.next());
                                if (r2->hasNext()) {
                                    aux2.write(r2->next());
                                } else {
                                    writer->write((IObject::Any &&) aux1.next());
                                    storage::iterator::readToWrite(*r1, *writer, true);
                                    break;
                                }
                            }
                        }
                        merges[i - 1] = obj;
                    }
                }
#pragma omp taskwait
                for (int i = 1; i < n; i++) {
                    merges.erase(merges.begin() + i);
                    n--;
                }
            }
        }
    }
    less.after(executor_data->getContext());
    executor_data->loadObject(merges[0]);
    IGNIS_LOG(info) << "ISortModule sorted";
}


void ISortModule::sampling(const int64_t sampleSize,const int64_t idx, const std::string &master) {
    try {
        IGNIS_LOG(info) << "ISortModule sampling";
        auto object_in = executor_data->loadObject();
        auto manager = getManager(*executor_data->loadObject());
        auto pivots = getIObject(manager, sampleSize, 0, "memory");
        auto reader = object_in->readIterator();
        auto writer = pivots->writeIterator();
        auto size = object_in->getSize();
        auto div = (size - sampleSize) / (sampleSize + 1);
        auto mod = (size - sampleSize) % (sampleSize + 1);

        for (int64_t i = 0; i < sampleSize; i++) {
            reader->skip(div + (i < mod ? 1 : 0));
            writer->write(reader->next());
        }
        executor_data->getPostBox().newOutMessage(idx, IMessage(master, pivots));
        IGNIS_LOG(info) << "ISortModule sampled";
    } catch (exceptions::IException &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack(ex.getStacktrace());
        throw iex;
    } catch (std::exception &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack("UNKNOWN");
        throw iex;
    }
}

void ISortModule::getPivots() {
    try {
        IGNIS_LOG(info) << "ISortModule getting pivots";
        auto msgs = executor_data->getPostBox().popInBox();
        auto manager = getManager(*executor_data->loadObject());
        auto pivots = getIObject(manager, msgs.size() * msgs.size(), 0, "memory");

        for (auto &entry: msgs) {
            entry.second.getObj()->setManager(manager);
            entry.second.getObj()->moveTo(*pivots);
        }
        executor_data->loadObject(pivots);
        IGNIS_LOG(info) << "ISortModule pivots got";
    } catch (exceptions::IException &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack(ex.getStacktrace());
        throw iex;
    } catch (std::exception &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack("UNKNOWN");
        throw iex;
    }
}

void ISortModule::findPivots(const std::vector<std::string> &nodes) {
    try {
        IGNIS_LOG(info) << "ISortModule finding pivots";
        auto pivots = executor_data->loadObject();
        auto manager = getManager(*executor_data->loadObject());
        auto node_pivots = getIObject(manager, nodes.size(), 0, "memory");
        auto reader = pivots->readIterator();
        auto writer = node_pivots->writeIterator();
        auto size = pivots->getSize();
        auto np = nodes.size() - 1;
        auto div = (size - np) / nodes.size();
        auto mod = (size - np) % nodes.size();

        for (int64_t i = 0; i < np; i++) {
            reader->skip(div + (i < mod ? 1 : 0));
            writer->write(reader->next());
        }

        for (size_t i = 0; i < nodes.size(); i++) {
            executor_data->getPostBox().newOutMessage(i, IMessage(nodes[i], node_pivots));
        }

        IGNIS_LOG(info) << "ISortModule pivots ready";
    } catch (exceptions::IException &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack(ex.getStacktrace());
        throw iex;
    } catch (std::exception &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack("UNKNOWN");
        throw iex;
    }
}

void ISortModule::exchangePartitions(const int64_t idx, const std::vector<std::string> &nodes) {
    try {
        IGNIS_LOG(info) << "ISortModule exchanging partitions";
        auto msgs = executor_data->getPostBox().popInBox();
        auto object_in = executor_data->loadObject();
        auto size = object_in->getSize();
        auto manager = object_in->getManager();
        std::vector<storage::iterator::IElementIterator> pivots;
        auto obj_msg = msgs.begin()->second.getObj();
        obj_msg->setManager(manager);
        auto reader_msg = obj_msg->readIterator();
        while (reader_msg->hasNext()) {
            pivots.emplace_back(manager);
            pivots.back().write(reader_msg->next());
        }

        std::vector<decltype(object_in)> partitions;
        std::vector<decltype(object_in->writeIterator())> writers;

        for (size_t i = 0; i < nodes.size(); i++) {
            partitions.push_back(getIObject(manager));
            writers.push_back(partitions.back()->writeIterator());
        }

        auto reader = object_in->readIterator();
        auto op = manager->_operator();
        for (size_t i = 0; i < size; i++) {
            auto &elem = reader->next();
            int init = 0;
            int end = pivots.size();
            while (init < end) {
                if (op->less(elem, pivots[(end - init) / 2].next())) {
                    end = (end - init) / 2;
                } else {
                    init = (end - init) / 2 + 1;
                }
            }
            writers[init]->write(elem);
        }

        for (size_t i = 0; i < nodes.size(); i++) {
            if (partitions[i]->getSize() > 0) {
                executor_data->getPostBox().newOutMessage(idx * nodes.size() + i, IMessage(nodes[i], partitions[i]));
            }
        }
        IGNIS_LOG(info) << "ISortModule partitions exchanged";
    } catch (exceptions::IException &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack(ex.getStacktrace());
        throw iex;
    } catch (std::exception &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack("UNKNOWN");
        throw iex;
    }
}

void ISortModule::mergePartitions() {
    try {
        IGNIS_LOG(info) << "ISortModule merging partitions";
        auto manager = getManager(*executor_data->loadObject());
        auto msgs = executor_data->getPostBox().popInBox();
        auto object_out = getIObject(manager);

        for (auto &entry: msgs) {
            entry.second.getObj()->setManager(manager);
            entry.second.getObj()->moveTo(*object_out);
        }
        executor_data->loadObject(object_out);
        IGNIS_LOG(info) << "IKeysModule partitions merged";
    } catch (exceptions::IException &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack(ex.getStacktrace());
        throw iex;
    } catch (std::exception &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack("UNKNOWN");
        throw iex;
    }
}

ISortModule::~ISortModule() {

}
