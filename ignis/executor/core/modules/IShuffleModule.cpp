
#include "IShuffleModule.h"
#include "../ILog.h"
#include "../storage/IMessage.h"
#include "../../../exceptions/IInvalidArgument.h"

using namespace std;
using namespace ignis::executor::core::modules;
using namespace ignis::executor::core::storage;
using namespace ignis::executor::core;
using ignis::rpc::IRemoteException;

IShuffleModule::IShuffleModule(shared_ptr<IExecutorData> &executor_data) : IgnisModule(executor_data) {}

void IShuffleModule::createSplits() {
    IGNIS_LOG(info) << "IShuffleModule started";
    try {
        it = executor_data->getLoadObject().readIterator();
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

void IShuffleModule::nextSplit(const std::string &host, const int32_t port, const int64_t length, const bool local) {
    IGNIS_LOG(info) << "IShuffleModule new split";
    try {
        shared_ptr<IObject> object = getIObject(executor_data->getLoadObject().getManager(), length);
        auto writer = object->writeIterator();
        readToWrite(*it, *writer, (size_t) length);
        if (local) {
            IGNIS_LOG(info) << "IShuffleModule split remote(host: " << host << ", port: " << port << "), length: "
                            << length;
            IMessage msg(host, port, object);
            executor_data->getPostBox().newMessage(msg);
        } else {
            IGNIS_LOG(info) << "IShuffleModule split local, length: " << length;
            IMessage msg(object, true);
            executor_data->getPostBox().newMessage(msg);
        }
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

void IShuffleModule::finishSplits() {
    IGNIS_LOG(info) << "IShuffleModule finished";
    try {
        it.reset();
        executor_data->deleteLoadObject();
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

IShuffleModule::~IShuffleModule() {
}
