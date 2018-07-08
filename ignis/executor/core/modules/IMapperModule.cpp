
#include "IMapperModule.h"
#include "../ILog.h"
#include "../storage/IThreadSplitter.h"
#include "../../../exceptions/IInvalidArgument.h"
#include "../IDinamicObject.h"
#include "../../api/function/IFunction.h"
#include "../../api/function/IFlatMapFunction.h"
#include "../storage/IMemoryObject.h"
#include <vector>

using namespace std;
using namespace ignis::executor::core::modules;
using namespace ignis::executor::core::storage;
using namespace ignis::executor::core;
using ignis::rpc::executor::IFunction;
using ignis::rpc::IRemoteException;

IMapperModule::IMapperModule(std::shared_ptr<IExecutorData> &executor_data) : IgnisModule(executor_data) {}

void IMapperModule::mapAux(const ::ignis::rpc::executor::IFunction &sf, bool flat) {
    try {
        auto function = loadFunction<api::function::IFunction<storage::IObject::Any, storage::IObject::Any>>(sf);
        auto object_in = executor_data->getSharedLoadObject();
        auto manager = (*function)->type_t.shared();
        auto manager_any = (std::shared_ptr<data::IManager<storage::IObject::Any>> &) manager;
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
                readToWrite<storage::IObject::Any>(*object_in->readIterator(), *object_aux->writeIterator());
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
                if (!flat) {
                    for (size_t i = 0; i < size_thread; i++) {
                        writer_thread->write(std::move((*function)->call(reader_thread->next(), context)));
                    }
                } else {
                    auto function_flat = (shared_ptr<api::function::IFlatMapFunction<storage::IObject::Any, vector<storage::IObject::Any>>> &) function;
                    while (reader_thread->hashNext()) {
                        auto iterable = (*function_flat).call(reader_thread->next(), context);
                        for (auto it = iterable.readIterator(); it->hashNext();) {
                            writer_thread->write(std::move(it->next()));
                        }
                    }
                }
#pragma omp ordered
                readToWrite<storage::IObject::Any>(*object_thread->readIterator(), *writer, true);
            }
        } else {
            auto reader = object_in->readIterator();
            if (!flat) {
                while (reader->hashNext()) {
                    writer->write(std::move((*function)->call(reader->next(), context)));
                }
            } else {
                auto function_flat = (shared_ptr<api::function::IFlatMapFunction<storage::IObject::Any, vector<storage::IObject::Any>>> &) function;
                while (reader->hashNext()) {
                    auto iterable = (*function_flat).call(reader->next(), context);
                    for (auto it = iterable.readIterator(); it->hashNext();) {
                        writer->write(std::move(it->next()));
                    }
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

void IMapperModule::_map(const ::ignis::rpc::executor::IFunction &sf) {
    IGNIS_LOG(info) << "IMapperModule starting map";
    mapAux(sf, false);
}

void IMapperModule::flatmap(const ::ignis::rpc::executor::IFunction &sf) {
    IGNIS_LOG(info) << "IMapperModule starting flatmap";
    mapAux(sf, true);
}

void IMapperModule::streamingMapAux(const ::ignis::rpc::executor::IFunction &sf, bool ordered, bool flat) {
    try {
        auto function = loadFunction<api::function::IFunction<storage::IObject::Any, storage::IObject::Any>>(sf);
        auto object_in = executor_data->getSharedLoadObject();
        auto manager = (*function)->type_t.shared();
        auto manager_any = (std::shared_ptr<data::IManager<storage::IObject::Any>> &) manager;
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
                if (!flat) {
                    while (reader_thread->hashNext()) {
                        writer_thread->write(std::move((*function)->call(reader_thread->next(), context)));
                    }
                } else {
                    auto function_flat = (shared_ptr<api::function::IFlatMapFunction<storage::IObject::Any, vector<storage::IObject::Any>>> &) function;
                    while (reader_thread->hashNext()) {
                        auto iterable = (*function_flat).call(reader_thread->next(), context);
                        for (auto it = iterable.readIterator(); it->hashNext();) {
                            writer_thread->write(std::move(it->next()));
                        }
                    }
                }
            }
        } else {
            auto reader = object_in->readIterator();
            auto writer = object_out->writeIterator();
            while (reader->hashNext()) {
                writer->write(std::move((*function)->call(reader->next(), context)));
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

void IMapperModule::streamingMap(const ::ignis::rpc::executor::IFunction &sf, bool ordered) {
    IGNIS_LOG(info) << "IMapperModule starting streaming map, mode: " << (ordered ? "ordered" : "unordered");
    streamingMapAux(sf, ordered, false);
}

void IMapperModule::streamingFlatmap(const ::ignis::rpc::executor::IFunction &sf, bool ordered) {
    IGNIS_LOG(info) << "IMapperModule starting streaming flatmap, mode: " << (ordered ? "ordered" : "unordered");
    streamingMapAux(sf, ordered, true);
}

IMapperModule::~IMapperModule() {

}
