
#include "IMapperModule.h"
#include "../ILog.h"
#include "../storage/IThreadSplitter.h"
#include "../../../exceptions/IInvalidArgument.h"
#include "../IDinamicObject.h"
#include "../../api/IMapper.h"
#include "../storage/IMemoryObject.h"

using namespace std;
using namespace ignis::executor::core::modules;
using namespace ignis::executor::core::storage;
using namespace ignis::executor::core;
using ignis::rpc::executor::IFunction;
using ignis::rpc::IRemoteException;

IMapperModule::IMapperModule(std::shared_ptr<IExecutorData> &executor_data) : IgnisModule(executor_data) {}


void IMapperModule::_map(const ::ignis::rpc::executor::IFunction &funct) {
    IGNIS_LOG(info) << "IMapperModule starting mapper";
    try {
        auto mapper = loadFunction<api::IMapper<IObject::Any, IObject::Any>>(funct);
        auto object_in = executor_data->getSharedLoadObject();
        auto manager = (*mapper)->type_t.shared();
        auto manager_any = (std::shared_ptr<data::IManager<IObject::Any>> &) manager;
        object_in->setManager(manager_any);
        auto object_out = getIObject(manager_any);
        size_t threads = executor_data->getThreads();
        (*mapper)->before(executor_data->getContext());
        if (threads > 1) {
            auto size = object_in->getSize();
            auto writer = object_out->writeIterator();
            if (object_in->getType() != "memory") {
                auto object_aux = getIObject(manager_any, object_in->getSize());
                readToWrite<IObject::Any>(*object_in->readIterator(), *object_aux->writeIterator());
                object_in = object_aux;
            }
#pragma omp parallel for num_threads(threads) ordered
            for (int t = 0; t < threads; t++) {
                auto object_thread = getIObject(manager_any,(size / threads) + 1);
                auto reader = object_in->readIterator();
                reader->skip((size / threads) * t + (((size % threads) < t) ? t : threads));
                (*mapper)->map(*reader, *object_thread->writeIterator(), executor_data->getContext());
#pragma omp ordered
                readToWrite<IObject::Any>(*object_thread->readIterator(), *writer, true);
            }
        } else {
            (*mapper)->map(*object_in->readIterator(), *object_out->writeIterator(), executor_data->getContext());
        }
        (*mapper)->after(executor_data->getContext());
        executor_data->loadObject(object_out);
        IGNIS_LOG(info) << "IMapperModule finished";
    } catch (exceptions::IException &ex) {
        IRemoteException iex;
        iex.__set_cause(ex.what());
        iex.__set_stack(ex.toString());
        throw iex;
    } catch (std::exception &ex) {
        IRemoteException iex;
        iex.__set_cause(ex.what());
        iex.__set_stack("UNKNOWN");
        throw iex;
    }
}

void IMapperModule::streamingMap(const ::ignis::rpc::executor::IFunction &funct, bool ordered) {
    try {
        IGNIS_LOG(info) << "IMapperModule starting streaming mapper, mode: " << (ordered ? "ordered" : "unordered");
        auto mapper = loadFunction<api::IMapper<IObject::Any, IObject::Any>>(funct);
        IObject &object_in = executor_data->getLoadObject();
        auto manager = (*mapper)->type_t.shared();
        object_in.setManager((std::shared_ptr<data::IManager<IObject::Any>> &) manager);
        auto object_out = getIObject(object_in.getManager());
        size_t threads = executor_data->getThreads();
        (*mapper)->before(executor_data->getContext());
        if (threads > 1) {
            auto buffer = executor_data->getParser().getNumber("ignis.executor.cores.buffer");
            IGNIS_LOG(info) << "IMapperModule creating " << threads << " threads";
            IThreadSplitter splitter(object_in, *object_out, threads, buffer, ordered);
#pragma omp parallel for num_threads(threads)
            for (int t = 0; t < threads; t++) {
                auto it = splitter[t];
                (*mapper)->map(*it.first, *it.second, executor_data->getContext());
            }
        } else {
            (*mapper)->map(*object_in.readIterator(), *object_out->writeIterator(), executor_data->getContext());
        }
        (*mapper)->after(executor_data->getContext());
        executor_data->loadObject(object_out);
        IGNIS_LOG(info) << "IMapperModule finished";
    } catch (exceptions::IException &ex) {
        IRemoteException iex;
        iex.__set_cause(ex.what());
        iex.__set_stack(ex.toString());
        throw iex;
    } catch (std::exception &ex) {
        IRemoteException iex;
        iex.__set_cause(ex.what());
        iex.__set_stack("UNKNOWN");
        throw iex;
    }
}

IMapperModule::~IMapperModule() {

}
