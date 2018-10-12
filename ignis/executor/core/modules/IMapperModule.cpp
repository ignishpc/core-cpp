
#include "IMapperModule.h"
#include <boost/lockfree/spsc_queue.hpp>
#include "../ILog.h"
#include "../IDinamicObject.h"
#include "../../../exceptions/IInvalidArgument.h"
#include "../../api/function/IFunction.h"
#include "../../api/function/IFlatFunction.h"
#include "../storage/IMemoryObject.h"


using namespace ignis::executor::core::modules;
using namespace ignis::executor::core::storage;
using namespace ignis::executor::core;
using ignis::rpc::IRemoteException;

IMapperModule::IMapperModule(std::shared_ptr<IExecutorData> &executor_data) : IgnisModule(executor_data) {}

template<template<typename...> typename F>
void IMapperModule::pipe(const rpc::ISourceFunction &sf, bool filter) {
    try {
        auto object_in = executor_data->getSharedLoadObject();
        executor_data->deleteLoadObject();
        auto function = loadFunction<F<IObject::Any, IObject::Any>>(sf);
        auto manager_t = (*function)->type_t.shared();
        auto manager_r = (*function)->type_r.shared();
        auto object_out = getIObject(manager_r);
        size_t threads = executor_data->getThreads();
        auto &context = executor_data->getContext();
        auto size = object_in->getSize();

        (*function)->before(executor_data->getContext());
        if (threads > 1) {
            IGNIS_LOG(info) << "IMapperModule converting storage to memory";
            if (object_in->getType() != storage::IMemoryObject::TYPE) {
                auto object_aux = getIObject(manager_t, object_in->getSize(), 0, storage::IMemoryObject::TYPE);
                object_in->moveTo(*object_aux);
                object_in = object_aux;
            }
            IGNIS_LOG(info) << "IMapperModule creating " << threads << " threads";
#pragma omp parallel for num_threads(threads) ordered
            for (int t = 0; t < threads; t++) {
                auto object_thread = getIObject(manager_t, (size / threads) + 1);
                auto reader_thread = object_in->readIterator();
                auto writer_thread = object_thread->writeIterator();
                auto size_thread = ((size / threads) * t + (((size % threads) < t) ? 1 : 0));
                reader_thread->skip((size / threads) * t + (((size % threads) < t) ? t : threads));
                if (filter) {
                    auto &function_filter = (api::function::IFunction<IObject::Any, bool> &) *function;
                    for (size_t i = 0; i < size; i++) {
                        auto &value = reader_thread->next();
                        if (function_filter.call(value, context)) {
                            writer_thread->write(value);
                        }
                    }
                } else {
                    for (size_t i = 0; i < size_thread; i++) {
                        (*function)->write(reader_thread->next(), context, *writer_thread);
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
            } else {
                for (size_t i = 0; i < size; i++) {
                    (*function)->write(reader->next(), context, *writer);
                }
            }
        }
        (*function)->after(executor_data->getContext());
        executor_data->loadObject(object_out);
        IGNIS_LOG(info) << "IMapperModule finished";
    } catch (exceptions::IException &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack(ex.toString());
        throw iex;
    } catch (std::exception &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack("UNKNOWN");
        throw iex;
    }
}


template<template<typename...> typename F>
void IMapperModule::streaming(const rpc::ISourceFunction &sf, bool ordered, bool filter) {
    try {
        auto object_in = executor_data->getSharedLoadObject();
        executor_data->deleteLoadObject();
        auto function = loadFunction<F<IObject::Any, IObject::Any>>(sf);
        auto manager_t = (*function)->type_t.shared();
        auto manager_r = (*function)->type_r.shared();
        auto object_out = getIObject(manager_r);
        size_t threads = executor_data->getThreads();
        auto &context = executor_data->getContext();

        (*function)->before(executor_data->getContext());
        if (threads > 1) {
            IGNIS_LOG(info) << "IMapperModule creating " << threads << " threads";
            auto buffer = executor_data->getParser().getNumber("ignis.executor.cores.buffer");
            auto reader = object_in->readIterator();
            boost::lockfree::spsc_queue<std::pair<bool, std::shared_ptr<storage::IObject>>> queue(10 * threads);
            bool empty = false;
#pragma omp parallel num_threads(threads)
            {
                std::vector<storage::IObject::Any *> elems;
                auto in_object_thread = getIObject(object_in->getManager(), 10, 0, storage::IMemoryObject::TYPE);
                auto out_object_thread = getIObject(object_in->getManager(), 10, 0, storage::IMemoryObject::TYPE);
                while (!empty) {
#pragma omp critical
                    {
                        empty = reader->hasNext();
                        if (!empty) {
                            if (object_in->getType() != storage::IMemoryObject::TYPE) {
                                storage::iterator::readToWrite(*reader, *in_object_thread->writeIterator(), buffer);
                                auto reader_thread = in_object_thread->readIterator();
                                while (reader_thread->hasNext()) {
                                    elems.push_back(&reader_thread->next());
                                }
                            } else {
                                for (int i = 0; i < buffer && reader->hasNext(); i++) {
                                    elems.push_back(&reader->next());
                                }
                            }
                            queue.push(
                                    std::pair<bool, std::shared_ptr<storage::IObject>>(false, out_object_thread));
                        }
                    }
                    if (!empty) {


                        auto writer_thread = out_object_thread->writeIterator();
                        if (filter) {
                            auto &function_filter = (api::function::IFunction<IObject::Any, bool> &) *function;
                            for (auto &elem: elems) {
                                if (function_filter.call(*elem, context)) {
                                    writer_thread->write(*elem);
                                }
                            }
                        } else {
                            for (auto &elem: elems) {
                                (*function)->write(*elem, context, *writer_thread);
                            }
                        }
#pragma omp critical
                        {
                            while (!queue.empty() && queue.front().first) {
                                auto &object = queue.front();
                                object.second->moveTo(*object_out);
                                queue.pop();
                            }
                        };
                    }
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
            } else {
                while (reader->hasNext()) {
                    (*function)->write(reader->next(), context, *writer);
                }
            }
        }
        (*function)->after(executor_data->getContext());
        IGNIS_LOG(info) << "IMapperModule finished";
    } catch (exceptions::IException &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack(ex.toString());
        throw iex;
    } catch (std::exception &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack("UNKNOWN");
        throw iex;
    }
}

void IMapperModule::_map(const rpc::ISourceFunction &sf) {
    IGNIS_LOG(info) << "IMapperModule starting map";
    pipe < api::function::IFunction > (sf, false);
}

void IMapperModule::flatmap(const rpc::ISourceFunction &sf) {
    IGNIS_LOG(info) << "IMapperModule starting flatmap";
    pipe < api::function::IFlatFunction > (sf, false);
}

void IMapperModule::filter(const rpc::ISourceFunction &sf) {
    IGNIS_LOG(info) << "IMapperModule starting filter";
    pipe < api::function::IFunction > (sf, true);
}

void IMapperModule::keyBy(const rpc::ISourceFunction &sf){

}

void IMapperModule::streamingMap(const rpc::ISourceFunction &sf, bool ordered) {
    IGNIS_LOG(info) << "IMapperModule starting streaming map, mode: " << (ordered ? "ordered" : "unordered");
    streaming<api::function::IFunction>(sf, ordered, false);
}

void IMapperModule::streamingFlatmap(const rpc::ISourceFunction &sf, bool ordered) {
    IGNIS_LOG(info) << "IMapperModule starting streaming flatmap, mode: " << (ordered ? "ordered" : "unordered");
    streaming<api::function::IFlatFunction>(sf, ordered, false);
}

void IMapperModule::streamingFilter(const rpc::ISourceFunction &sf, bool ordered) {
    IGNIS_LOG(info) << "IMapperModule starting streaming filter, mode: " << (ordered ? "ordered" : "unordered");
    streaming<api::function::IFunction>(sf, ordered, true);
}

IMapperModule::~IMapperModule() {}
