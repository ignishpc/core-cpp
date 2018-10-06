
#include "IMapperModule.h"
#include "../ILog.h"
#include "../storage/IThreadSplitter.h"
#include "../../../exceptions/IInvalidArgument.h"
#include "../IDinamicObject.h"
#include "../../api/function/IFunction.h"
#include "../../api/function/IFlatFunction.h"

using namespace ignis::executor::core::modules;
using namespace ignis::executor::core::storage;
using namespace ignis::executor::core;
using ignis::rpc::IRemoteException;

IMapperModule::IMapperModule(std::shared_ptr<IExecutorData> &executor_data) : IgnisModule(executor_data) {}

void IMapperModule::pipe(const rpc::ISourceFunction &sf, bool filter) {
    try {
        auto function = loadFunction<api::function::IFunction<IObject::Any, IObject::Any>>(sf);
        auto object_in = executor_data->getSharedLoadObject();
        auto manager_t = (*function)->type_t.shared();
        auto manager_r = (*function)->type_r.shared();
        auto manager_t_any = (std::shared_ptr<data::IManager<IObject::Any>> &) manager_t;
        auto manager_r_any = (std::shared_ptr<data::IManager<IObject::Any>> &) manager_r;
        object_in->setManager(manager_t_any);
        auto object_out = getIObject(manager_r_any);
        size_t threads = executor_data->getThreads();
        auto writer = object_out->writeIterator();
        auto &context = executor_data->getContext();
        auto size = object_in->getSize();
        executor_data->loadObject(object_out);

        (*function)->before(executor_data->getContext());
        if (threads > 1) {
            if (object_in->getType() != "memory") {
                auto object_aux = getIObject(manager_t_any, object_in->getSize(), 0, "memory");
                iterator::readToWrite<IObject::Any>(*object_in->readIterator(), *object_aux->writeIterator());
                object_in = object_aux;
            }
            IGNIS_LOG(info) << "IMapperModule creating " << threads << " threads";
#pragma omp parallel for num_threads(threads) ordered
            for (int t = 0; t < threads; t++) {
                auto object_thread = getIObject(manager_r_any, (size / threads) + 1, 0, "memory");
                auto reader_thread = object_in->readIterator();
                auto writer_thread = object_thread->writeIterator();
                auto size_thread = ((size / threads) * t + (((size % threads) < t) ? 1 : 0));
                reader_thread->skip((size / threads) * t + (((size % threads) < t) ? t : threads));
                if (filter) {
                    auto &function_filter = (api::function::IFunction<IObject::Any, bool> &) *function;
                    for (size_t i = 0; i < size_thread; i++) {
                        auto &value = reader_thread->next();
                        if (function_filter.call(value, context)) {
                            writer_thread->write(value);
                        }
                    }
                } else {
                    for (size_t i = 0; i < size_thread; i++) {
                        (*function)->writeCall(reader_thread->next(), context, *writer_thread);
                    }
                }
#pragma omp ordered
                iterator::readToWrite<IObject::Any>(*object_thread->readIterator(), *writer, true);
            }
        } else {
            auto reader = object_in->readIterator();
            if (filter) {
                auto &function_filter = (api::function::IFunction<IObject::Any, bool> &) *function;
                while (reader->hashNext()) {
                    auto &value = reader->next();
                    if (function_filter.call(value, context)) {
                        writer->write(value);
                    }
                }
            } else {
                (*function)->writeCall(reader->next(), context, *writer);
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

void IMapperModule::streaming(const rpc::ISourceFunction &sf, bool ordered, bool filter) {
    try {
        auto function = loadFunction<api::function::IFunction<IObject::Any, IObject::Any>>(sf);
        auto object_in = executor_data->getSharedLoadObject();
        auto manager_t = (*function)->type_t.shared();
        auto manager_r = (*function)->type_r.shared();
        auto manager_t_any = (std::shared_ptr<data::IManager<IObject::Any>> &) manager_t;
        auto manager_r_any = (std::shared_ptr<data::IManager<IObject::Any>> &) manager_r;
        object_in->setManager(manager_t_any);
        auto object_out = getIObject(manager_r_any);
        size_t threads = executor_data->getThreads();
        auto &context = executor_data->getContext();
        executor_data->loadObject(object_out);

        (*function)->before(executor_data->getContext());
        if (threads > 1) {
            auto buffer = executor_data->getParser().getNumber("ignis.executor.cores.buffer");
            IGNIS_LOG(info) << "IMapperModule creating " << threads << " threads";
            IThreadSplitter splitter(*object_in, *object_out, threads, buffer, ordered);
#pragma omp parallel for num_threads(threads)
            for (int t = 0; t < threads; t++) {
                auto it = splitter[t];
                auto reader_thread = it.first;
                auto writer_thread = it.second;
                if (filter) {
                    auto &function_filter = (api::function::IFunction<IObject::Any, bool> &) *function;
                    while (reader_thread->hashNext()) {
                        auto &value = reader_thread->next();
                        if (function_filter.call(value, context)) {
                            writer_thread->write(value);
                        }
                    }
                } else {
                    while (reader_thread->hashNext()) {
                        (*function)->writeCall(reader_thread->next(), context, *writer_thread);
                    }
                }
            }
        } else {
            auto reader = object_in->readIterator();
            auto writer = object_out->writeIterator();
            if (filter) {
                auto &function_filter = (api::function::IFunction<IObject::Any, bool> &) *function;
                while (reader->hashNext()) {
                    auto &value = reader->next();
                    if (function_filter.call(value, context)) {
                        writer->write(value);
                    }
                }
            } else {
                (*function)->writeCall(reader->next(), context, *writer);
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
    pipe(sf, false);
}

void IMapperModule::flatmap(const rpc::ISourceFunction &sf) {
    IGNIS_LOG(info) << "IMapperModule starting flatmap";
    pipe(sf, false);
}

void IMapperModule::filter(const rpc::ISourceFunction &sf) {
    IGNIS_LOG(info) << "IMapperModule starting filter";
    pipe(sf, true);
}

void IMapperModule::streamingMap(const rpc::ISourceFunction &sf, bool ordered) {
    IGNIS_LOG(info) << "IMapperModule starting streaming map, mode: " << (ordered ? "ordered" : "unordered");
    streaming(sf, ordered, false);
}

void IMapperModule::streamingFlatmap(const rpc::ISourceFunction &sf, bool ordered) {
    IGNIS_LOG(info) << "IMapperModule starting streaming flatmap, mode: " << (ordered ? "ordered" : "unordered");
    streaming(sf, ordered, false);
}

void IMapperModule::streamingFilter(const rpc::ISourceFunction &sf, bool ordered) {
    IGNIS_LOG(info) << "IMapperModule starting streaming filter, mode: " << (ordered ? "ordered" : "unordered");
    streaming(sf, ordered, true);
}

IMapperModule::~IMapperModule() {}
