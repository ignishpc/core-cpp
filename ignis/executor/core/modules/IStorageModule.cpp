
#include "IStorageModule.h"
#include "../../../exceptions/IInvalidArgument.h"
#include "../../../data/IMemoryBuffer.h"
#include "../IMessage.h"
#include "../ILog.h"

using namespace ignis::executor::core::modules;
using namespace ignis::executor::core;
using ignis::rpc::IRemoteException;


IStorageModule::IStorageModule(std::shared_ptr<IExecutorData> &executor_data) : IgnisModule(executor_data) {}

IStorageModule::~IStorageModule() {

}

int64_t IStorageModule::count() {
    return executor_data->loadObject()->getSize();
}

void IStorageModule::cache(const int64_t id) {
    try {
        IGNIS_LOG(info) << "IStorageModule loading cache object " << id;
        objects_cache[id] = executor_data->loadObject();
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

void IStorageModule::uncache(const int64_t id) {
    try {
        IGNIS_LOG(info) << "IStorageModule deleting cache object " << id;
        objects_cache.erase(id);
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

void IStorageModule::loadCache(const int64_t id) {
    try {
        IGNIS_LOG(info) << "IStorageModule loading cache object " << id;
        if (objects_cache.find(id) == objects_cache.end()) {
            throw exceptions::IInvalidArgument("IStorageModule cache object not found");
        }
        executor_data->loadObject(objects_cache[id]);
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

void IStorageModule::saveContext(const int64_t id) {
    try {
        objects_context[id] = executor_data->loadObject();
        IGNIS_LOG(info) << "IStorageModule context " << id << " saved";
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

void IStorageModule::loadContext(const int64_t id) {
    try {
        if (objects_context.find(id) == objects_context.end()) {
            throw exceptions::IInvalidArgument("IStorageModule context object not found");
        }
        bool already_loaded = objects_context[id] == executor_data->loadObject();

        if (!already_loaded) {
            executor_data->loadObject(objects_context[id]);
            IGNIS_LOG(info) << "IStorageModule context " << id << " loaded";
        }
        objects_context.erase(id);

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

void IStorageModule::take(std::string& _return, const int64_t msg_id, const std::string& addr, const int64_t n, const bool light) {
    IGNIS_LOG(info) << "IMapperModule starting take, n: " << n << ", light: " << (light ? "ordered" : "unordered");
    try {
        auto loaded = executor_data->loadObject();
        executor_data->deleteLoadObject();
        auto object = getIObject(loaded->getManager(),n);
        storage::iterator::readToWrite(*loaded->readIterator(), *object->writeIterator(), n, false);

        if(light){
            auto buffer = std::make_shared<data::IMemoryBuffer>();
            object->write(buffer, 0);//rpc already has compression
            _return = buffer->getBufferAsString();
        }else{
            executor_data->getPostBox().newOutMessage(msg_id, IMessage(addr, object));
        }
        IGNIS_LOG(info) << "IMapperModule take done";
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

void IStorageModule::takeSample(std::string& _return, const int64_t msg_id, const std::string& addr, const int64_t n, const bool withRemplacement, const int32_t seed, const bool light) {
    IGNIS_LOG(info) << "IMapperModule starting takeSample, n: " << n
                    << ", withRemplacement: " << (withRemplacement ? "ordered" : "unordered")
                    << ", seed: " << seed
                    << ", light: " << (light ? "ordered" : "unordered");
    try {
        auto loaded = executor_data->loadObject();
        executor_data->deleteLoadObject();
        auto object = getIObject(loaded->getManager(),n);

        // TODO
        if(light){
            auto buffer = std::make_shared<data::IMemoryBuffer>();
            object->write(buffer, 0);//rpc already has compression
            _return = buffer->getBufferAsString();
        }else{
            executor_data->getPostBox().newOutMessage(msg_id, IMessage(addr, object));
        }
        IGNIS_LOG(info) << "IMapperModule takeSample done";
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

void IStorageModule::collect(std::string& _return, const int64_t msg_id, const std::string& addr, const bool light) {
    IGNIS_LOG(info) << "IMapperModule starting collect, light: " << (light ? "ordered" : "unordered");
    try {
        auto object = executor_data->loadObject();
        executor_data->deleteLoadObject();
        if(light){
            auto buffer = std::make_shared<data::IMemoryBuffer>();
            object->write(buffer, 0);//rpc already has compression
            _return = buffer->getBufferAsString();
        }else{
            executor_data->getPostBox().newOutMessage(msg_id, IMessage(addr, object));
        }
        IGNIS_LOG(info) << "IMapperModule collect done";
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
