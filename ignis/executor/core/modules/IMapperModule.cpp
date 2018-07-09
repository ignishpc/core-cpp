
#include "IMapperModule.h"
#include "../ILog.h"
#include "../storage/IThreadSplitter.h"
#include "../../../exceptions/IInvalidArgument.h"
#include "../IDinamicObject.h"
#include "../../api/function/IFunction.h"
#include "../../api/function/IFlatFunction.h"
#include "../storage/IMemoryObject.h"
#include <vector>

#define MODE_NORMAL 0
#define MODE_FLAT 1
#define MODE_FILTER 2

using namespace std;
using namespace ignis::executor::core::modules;
using namespace ignis::executor::core::storage;
using namespace ignis::executor::core;
using ignis::rpc::IRemoteException;

IMapperModule::IMapperModule(std::shared_ptr<IExecutorData> &executor_data) : IgnisModule(executor_data) {}

void IMapperModule::pipe(const rpc::executor::IFunction& sf, int8_t mode){
    try {
        auto function = loadFunction<api::function::IFunction<IObject::Any, IObject::Any>>(sf);
        auto object_in = executor_data->getSharedLoadObject();
        auto manager = (*function)->type_t.shared();
        auto manager_any = (std::shared_ptr<data::IManager<IObject::Any>> &) manager;
        object_in->setManager(manager_any);
        auto object_out = getIObject(manager_any);
        size_t threads = executor_data->getThreads();
        auto writer = object_out->writeIterator();
        auto &context = executor_data->getContext();
        auto size = object_in->getSize();

        (*function)->before(executor_data->getContext());
        if (threads > 1) {
            if (object_in->getType() != "memory") {
                auto object_aux = getIObject(manager_any, object_in->getSize());
                readToWrite<IObject::Any>(*object_in->readIterator(), *object_aux->writeIterator());
                object_in = object_aux;
            }
            IGNIS_LOG(info) << "IMapperModule creating " << threads << " threads";
#pragma omp parallel for num_threads(threads) ordered
            for (int t = 0; t < threads; t++) {
                auto object_thread = getIObject(manager_any, (size / threads) + 1, 0, "memory");
                auto reader_thread = object_in->readIterator();
                auto writer_thread = object_thread->writeIterator();
                auto size_thread = ((size / threads) * t + (((size % threads) < t) ? 1 : 0));
                reader_thread->skip((size / threads) * t + (((size % threads) < t) ? t : threads));
                if(mode == MODE_FLAT){
                    auto& function_flat = (api::function::IFlatFunction<IObject::Any, vector<IObject::Any>> &) *function;
                    for (size_t i = 0; i < size_thread; i++) {
                        auto iterable = function_flat.call(reader_thread->next(), context);
                        for (auto it = iterable.readIterator(); it->hashNext();) {
                            writer_thread->write(std::move(it->next()));
                        }
                    }
                }else if(mode == MODE_FILTER){
                    auto& function_filter = (api::function::IFunction<IObject::Any, bool> &) *function;
                    for (size_t i = 0; i < size_thread; i++) {
                        auto &value = reader_thread->next();
                        if(function_filter.call(value, context)){
                            writer_thread->write(value);
                        }
                    }
                }else{
                    for (size_t i = 0; i < size_thread; i++) {
                        writer_thread->write(std::move((*function)->call(reader_thread->next(), context)));
                    }
                }
#pragma omp ordered
                readToWrite<IObject::Any>(*object_thread->readIterator(), *writer, true);
            }
        } else {
            auto reader = object_in->readIterator();
            if(mode == MODE_FLAT){
                auto& function_flat = (api::function::IFlatFunction<IObject::Any, vector<IObject::Any>> &) *function;
                while (reader->hashNext()) {
                    auto iterable = function_flat.call(reader->next(), context);
                    for (auto it = iterable.readIterator(); it->hashNext();) {
                        writer->write(std::move(it->next()));
                    }
                }
            }else if(mode == MODE_FILTER){
                auto& function_filter = (api::function::IFunction<IObject::Any, bool> &) *function;
                while (reader->hashNext()) {
                    auto &value = reader->next();
                    if(function_filter.call(value, context)){
                        writer->write(value);
                    }
                }
            }else{
                while (reader->hashNext()) {
                    writer->write(std::move((*function)->call(reader->next(), context)));
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

void IMapperModule::streaming(const rpc::executor::IFunction &sf, bool ordered, int8_t mode) {
    try {
        auto function = loadFunction<api::function::IFunction<IObject::Any, IObject::Any>>(sf);
        auto object_in = executor_data->getSharedLoadObject();
        auto manager = (*function)->type_t.shared();
        auto manager_any = (std::shared_ptr<data::IManager<IObject::Any>> &) manager;
        object_in->setManager(manager_any);
        auto object_out = getIObject(manager_any);
        size_t threads = executor_data->getThreads();
        auto &context = executor_data->getContext();

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
                if(mode == MODE_FLAT){
                    auto& function_flat = (api::function::IFlatFunction<IObject::Any, vector<IObject::Any>> &) *function;
                    while (reader_thread->hashNext()) {
                        auto iterable = function_flat.call(reader_thread->next(), context);
                        for (auto it = iterable.readIterator(); it->hashNext();) {
                            writer_thread->write(std::move(it->next()));
                        }
                    }
                }else if(mode == MODE_FILTER){
                    auto& function_filter = (api::function::IFunction<IObject::Any, bool> &) *function;
                    while (reader_thread->hashNext()) {
                        auto &value = reader_thread->next();
                        if(function_filter.call(value, context)){
                            writer_thread->write(value);
                        }
                    }
                }else{
                    while (reader_thread->hashNext()) {
                        writer_thread->write(std::move((*function)->call(reader_thread->next(), context)));
                    }
                }
            }
        } else {
            auto reader = object_in->readIterator();
            auto writer = object_out->writeIterator();
            if(mode == MODE_FLAT){
                auto& function_flat = (api::function::IFlatFunction<IObject::Any, vector<IObject::Any>> &) *function;
                while (reader->hashNext()) {
                    auto iterable = function_flat.call(reader->next(), context);
                    for (auto it = iterable.readIterator(); it->hashNext();) {
                        writer->write(std::move(it->next()));
                    }
                }
            }else if(mode == MODE_FILTER){
                auto& function_filter = (api::function::IFunction<IObject::Any, bool> &) *function;
                while (reader->hashNext()) {
                    auto &value = reader->next();
                    if(function_filter.call(value, context)){
                        writer->write(value);
                    }
                }
            }else{
                while (reader->hashNext()) {
                    writer->write(std::move((*function)->call(reader->next(), context)));
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

void IMapperModule::_map(const rpc::executor::IFunction &sf) {
    IGNIS_LOG(info) << "IMapperModule starting map";
    pipe(sf, MODE_NORMAL);
}

void IMapperModule::flatmap(const rpc::executor::IFunction &sf) {
    IGNIS_LOG(info) << "IMapperModule starting flatmap";
    pipe(sf, MODE_FLAT);
}

void IMapperModule::filter(const  rpc::executor::IFunction& sf){
    IGNIS_LOG(info) << "IMapperModule starting filter";
    pipe(sf, MODE_FILTER);
}

void IMapperModule::streamingMap(const rpc::executor::IFunction &sf, bool ordered) {
    IGNIS_LOG(info) << "IMapperModule starting streaming map, mode: " << (ordered ? "ordered" : "unordered");
    streaming(sf, ordered, MODE_NORMAL);
}

void IMapperModule::streamingFlatmap(const rpc::executor::IFunction &sf, bool ordered) {
    IGNIS_LOG(info) << "IMapperModule starting streaming flatmap, mode: " << (ordered ? "ordered" : "unordered");
    streaming(sf, ordered, MODE_FLAT);
}

void IMapperModule::streamingFilter(const  rpc::executor::IFunction& sf, bool ordered){
    IGNIS_LOG(info) << "IMapperModule starting streaming filter, mode: " << (ordered ? "ordered" : "unordered");
    streaming(sf, ordered, MODE_FILTER);
}

IMapperModule::~IMapperModule() {}
