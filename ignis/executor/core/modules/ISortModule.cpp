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

void ISortModule::localCustomSort(const ignis::rpc::ISource &sf, bool ascending) {
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

void ISortModule::localSort(bool ascending) {
    try {
        NarutalOrder less(executor_data->loadObject()->getManager()->_operator());
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

void ISortModule::mergeSort(function::IFunction2<IObject::Any, IObject::Any, bool> &less, bool ascending) {
    IGNIS_LOG(info) << "ISortModule sorting";
    auto object_in = executor_data->loadObject();
    auto manager = object_in->getManager();
    auto object_out = getIObject(manager);
    auto size = object_in->getSize();
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
    threads = std::min(threads, (size_t)size / 10);
    threads = std::max(threads, (size_t)1);
    IGNIS_LOG(info) << "ISortModule sorting with " << threads << " threads";
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

    executor_data->loadObject(merges[0]);
    IGNIS_LOG(info) << "ISortModule sorted";
}

ISortModule::~ISortModule() {

}
