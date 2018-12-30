
#include "IMapperModule.h"
#include <list>
#include <mutex>
#include "../ILog.h"
#include "../IObjectLoader.h"
#include "../../../exceptions/IInvalidArgument.h"
#include "../../api/function/IFunction.h"
#include "../../api/function/IFlatFunction.h"
#include "../storage/IMemoryObject.h"


using namespace ignis::executor::core::modules;
using namespace ignis::executor::core::storage;
using namespace ignis::executor::core;
using ignis::rpc::IRemoteException;

IMapperModule::IMapperModule(std::shared_ptr<IExecutorData> &executor_data) : IgnisModule(executor_data) {}

template<template<typename...> class F, bool filter, bool key>
void IMapperModule::pipe(const rpc::ISource &sf) {
    try {
        auto object_in = executor_data->loadObject();
        executor_data->deleteLoadObject();
        auto function = loadSource<F<IObject::Any, IObject::Any>>(sf);
        auto manager_t = function->type_t();
        auto manager_r = function->type_r();
        if (key) {
            auto manager_rt = ((api::function::IFunction<IObject::Any, IObject::Any> *) function.get())->pair_rt();
            manager_r = (decltype(manager_r) &) manager_rt;
        } else if (filter) {
            manager_r = manager_t;
        }
        auto object_out = getIObject(manager_r);
        object_in->setManager(manager_t);
        size_t threads = executor_data->getThreads();
        auto &context = executor_data->getContext();
        auto size = object_in->getSize();

        function->before(executor_data->getContext());
        if (threads > 1) {
            auto reader = object_in->readIterator();
            decltype(object_in) parts[threads];
            decltype(reader) readers[threads];
            auto div = size / threads;
            auto mod = size % threads;
            if (object_in->getType() == storage::IMemoryObject::TYPE) {
                for (int t = 0; t < threads; t++) {
                    readers[t] = object_in->readIterator();
                    readers[t]->skip(t * div + (t < mod ? t : mod));
                }
            } else {
                for (int t = 0; t < threads; t++) {
                    parts[t] = getIObject(manager_t, div + 1);
                    storage::iterator::readToWrite(*reader, *parts[t]->writeIterator(), div + (t < mod ? 1 : 0));
                    readers[t] = parts[t]->readIterator();
                }
                object_in = parts[0];
            }

            IGNIS_LOG(info) << "IMapperModule creating " << threads << " threads";
#pragma omp parallel for num_threads(threads) ordered
            for (int t = 0; t < threads; t++) {
                auto size_thread = size / threads + (t < mod ? 1 : 0);
                auto object_thread = getIObject(manager_r, size_thread);
                auto reader_thread = readers[t];
                auto writer_thread = object_thread->writeIterator();
                if (filter) {
                    auto &function_filter = (api::function::IFunction<IObject::Any, bool> &) *function;
                    for (size_t i = 0; i < size_thread; i++) {
                        auto &value = reader_thread->next();
                        if (function_filter.call(value, context)) {
                            writer_thread->write(value);
                        }
                    }
                } else if (key) {
                    auto &function_key = (api::function::IFunction<IObject::Any, IObject::Any> &) *function;
                    auto &pair_writer = (storage::iterator::ICoreWriteIterator<typename api::IPairManager<IObject::Any,
                            IObject::Any>::Class> &) *writer_thread;
                    for (size_t i = 0; i < size_thread; i++) {
                        function_key.write(reader_thread->next(), context, pair_writer);
                    }
                } else {
                    for (size_t i = 0; i < size_thread; i++) {
                        function->write(reader_thread->next(), context, *writer_thread);
                    }
                }
#pragma omp ordered
                object_thread->moveTo(*object_out);
            }
        } else {
            auto reader = object_in->readIterator();
            auto writer = object_out->writeIterator();
            if (filter) {
                auto &function_filter = (api::function::IFunction<IObject::Any, bool> &) *function;
                for (size_t i = 0; i < size; i++) {
                    auto &value = reader->next();
                    if (function_filter.call(value, context)) {
                        writer->write(value);
                    }
                }
            } else if (key) {
                auto &function_key = (api::function::IFunction<IObject::Any, IObject::Any> &) *function;
                auto &pair_writer = (storage::iterator::ICoreWriteIterator<typename api::IPairManager<IObject::Any,
                        IObject::Any>::Class> &) *writer;
                for (size_t i = 0; i < size; i++) {
                    function_key.write(reader->next(), context, pair_writer);
                }
            } else {
                for (size_t i = 0; i < size; i++) {
                    function->write(reader->next(), context, *writer);
                }
            }
        }
        function->after(executor_data->getContext());
        object_out->fit();
        executor_data->loadObject(object_out);
        IGNIS_LOG(info) << "IMapperModule finished";
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


template<template<typename...> class F, bool filter, bool key>
void IMapperModule::streaming(const rpc::ISource &sf, bool ordered) {
    try {
        auto object_in = executor_data->loadObject();
        executor_data->deleteLoadObject();
        auto function = loadSource<F<IObject::Any, IObject::Any>>(sf);
        auto manager_t = function->type_t();
        auto manager_r = function->type_r();
        if (key) {
            auto manager_rt = ((api::function::IFunction<IObject::Any, IObject::Any> *) function.get())->pair_rt();
            manager_r = (decltype(manager_r) &) manager_rt;
        } else if (filter) {
            manager_r = manager_t;
        }
        auto object_out = getIObject(manager_r);
        object_in->setManager(manager_t);
        size_t threads = executor_data->getThreads();
        auto &context = executor_data->getContext();

        function->before(executor_data->getContext());
        if (threads > 1) {
            IGNIS_LOG(info) << "IMapperModule creating " << threads << " threads";
            auto chunk = executor_data->getParser().getNumber("ignis.executor.cores.chunk");
            auto reader = object_in->readIterator();
            std::list<std::pair<std::shared_ptr<bool>, std::shared_ptr<storage::IObject>>> result_queue;
            std::mutex mutex;
#pragma omp parallel num_threads(threads)
            {
                auto out_object_thread = getIObject(object_out->getManager(), chunk, 0, storage::IMemoryObject::TYPE);
                auto in_object_thread = getIObject(object_in->getManager(), chunk, 0, storage::IMemoryObject::TYPE);
                std::shared_ptr<bool> object_check;
                bool empty = false;
                while (!empty) {
#pragma omp critical
                    {
                        empty = !reader->hasNext();
                        if (!empty) {
                            storage::iterator::readToWrite(*reader, *in_object_thread->writeIterator(), chunk);
                            if (ordered) {
                                object_check = std::make_shared<bool>(false);
                                {
                                    std::lock_guard<std::mutex> lock(mutex);
                                    result_queue.emplace_back(object_check, out_object_thread);
                                }
                            }
                        }
                    }
                    if (!empty) {
                        auto writer_thread = out_object_thread->writeIterator();
                        auto reader_thread = in_object_thread->readIterator();
                        if (filter) {
                            auto &function_filter = (api::function::IFunction<IObject::Any, bool> &) *function;
                            while (reader_thread->hasNext()) {
                                auto &elem = reader_thread->next();
                                if (function_filter.call(elem, context)) {
                                    writer_thread->write(elem);
                                }
                            }
                        } else if (key) {
                            auto &function_key = (api::function::IFunction<IObject::Any, IObject::Any> &) *function;
                            auto &pair_writer = (storage::iterator::ICoreWriteIterator<typename api::IPairManager<IObject::Any,
                                    IObject::Any>::Class> &) *writer_thread;
                            while (reader_thread->hasNext()) {
                                function_key.write(reader_thread->next(), context, pair_writer);
                            }
                        } else {
                            while (reader_thread->hasNext()) {
                                function->write(reader_thread->next(), context, *writer_thread);
                            }
                        }
#pragma omp critical
                        {
                            if (ordered) {
                                *object_check = true;
                                out_object_thread =
                                        getIObject(object_out->getManager(), chunk, 0, storage::IMemoryObject::TYPE);
                                while (!result_queue.empty() && *(result_queue.front().first)) {
                                    auto &object = result_queue.front();
                                    object.second->moveTo(*object_out);
                                    {
                                        std::lock_guard<std::mutex> lock(mutex);
                                        result_queue.pop_front();
                                    }
                                }
                            } else {
                                out_object_thread->moveTo(*object_out);
                            }
                        };
                    }
                    in_object_thread->clear();
                }
            }
        } else {
            auto reader = object_in->readIterator();
            auto writer = object_out->writeIterator();
            if (filter) {
                auto &function_filter = (api::function::IFunction<IObject::Any, bool> &) *function;
                while (reader->hasNext()) {
                    auto &value = reader->next();
                    if (function_filter.call(value, context)) {
                        writer->write(value);
                    }
                }
            } else if (key) {
                auto &function_key = (api::function::IFunction<IObject::Any, IObject::Any> &) *function;
                auto &pair_writer = (storage::iterator::ICoreWriteIterator<typename api::IPairManager<IObject::Any,
                        IObject::Any>::Class> &) *writer;
                while (reader->hasNext()) {
                    function_key.write(reader->next(), context, pair_writer);
                }
            } else {
                while (reader->hasNext()) {
                    function->write(reader->next(), context, *writer);
                }
            }
        }
        function->after(executor_data->getContext());
        object_out->fit();
        executor_data->loadObject(object_out);
        IGNIS_LOG(info) << "IMapperModule finished";
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

void IMapperModule::_map(const rpc::ISource &sf) {
    IGNIS_LOG(info) << "IMapperModule starting map";
    pipe < api::function::IFunction > (sf);
}

void IMapperModule::flatmap(const rpc::ISource &sf) {
    IGNIS_LOG(info) << "IMapperModule starting flatmap";
    pipe < api::function::IFlatFunction > (sf);
}

void IMapperModule::filter(const rpc::ISource &sf) {
    IGNIS_LOG(info) << "IMapperModule starting filter";
    pipe < api::function::IFunction, true > (sf);
}

void IMapperModule::keyBy(const rpc::ISource &sf) {
    IGNIS_LOG(info) << "IMapperModule starting keyBy";
    pipe < api::function::IFunction, false, true > (sf);
}

void IMapperModule::streamingMap(const rpc::ISource &sf, bool ordered) {
    IGNIS_LOG(info) << "IMapperModule starting streaming map, mode: " << (ordered ? "ordered" : "unordered");
    streaming<api::function::IFunction, false>(sf, ordered);
}

void IMapperModule::streamingFlatmap(const rpc::ISource &sf, bool ordered) {
    IGNIS_LOG(info) << "IMapperModule starting streaming flatmap, mode: " << (ordered ? "ordered" : "unordered");
    streaming<api::function::IFlatFunction, false>(sf, ordered);
}

void IMapperModule::streamingFilter(const rpc::ISource &sf, bool ordered) {
    IGNIS_LOG(info) << "IMapperModule starting streaming filter, mode: " << (ordered ? "ordered" : "unordered");
    streaming<api::function::IFunction, true>(sf, ordered);
}

void IMapperModule::streamingKeyBy(const rpc::ISource &sf, bool ordered) {
    IGNIS_LOG(info) << "IMapperModule starting streaming keyBy, mode: " << (ordered ? "ordered" : "unordered");
    streaming<api::function::IFunction, false, true>(sf, ordered);
}

void IMapperModule::values() {
    IGNIS_LOG(info) << "IMapperModule starting values";
    try {
        auto object_in = executor_data->loadObject();
        auto manager = getManager(*object_in);
        auto pair_manager = (std::shared_ptr<api::IPairManager<storage::IObject::Any, storage::IObject::Any>> &) manager;
        auto manager_r = pair_manager->secondManager();
        auto size = object_in->getSize();
        auto object_out = getIObject(manager_r, size);

        auto reader = object_in->readIterator();
        auto writer = object_out->writeIterator();
        for (size_t i = 0; i < size; i++) {
            auto &pair = (std::pair<storage::IObject::Any, storage::IObject::Any> &) reader->next();
            writer->write(pair_manager->second(pair));
        }
        executor_data->loadObject(object_out);
        IGNIS_LOG(info) << "IMapperModule finished";
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

IMapperModule::~IMapperModule() {}


