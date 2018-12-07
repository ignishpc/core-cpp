
#include "IShuffleModule.h"
#include "../ILog.h"
#include "../IMessage.h"
#include "../../../exceptions/IInvalidArgument.h"

using namespace ignis::executor::core::modules;
using namespace ignis::executor::core::storage;
using namespace ignis::executor::core;
using ignis::rpc::IRemoteException;

IShuffleModule::IShuffleModule(std::shared_ptr<IExecutorData> &executor_data) : IgnisModule(executor_data) {}

void IShuffleModule::createSplits(const std::vector<ignis::rpc::executor::ISplit> &splits) {
    IGNIS_LOG(info) << "IShuffleModule started";
    try {
        auto object = executor_data->loadObject();
        if(splits.size() == 1 && object->getSize() == splits[0].length){
            executor_data->getPostBox().newOutMessage(splits[0].msg_id, IMessage(splits[0].addr, object));
            IGNIS_LOG(info) << "IShuffleModule split addr: " << splits[0].addr << ", length: " << splits[0].length;
        }

        auto manager = getManager(*object);

        auto reader = object->readIterator();
        for (auto &split : splits) {
            auto split_object = getIObject(manager, split.length);
            storage::iterator::readToWrite(*reader, *split_object->writeIterator(), split.length, true);
            executor_data->getPostBox().newOutMessage(split.msg_id, IMessage(split.addr, split_object));
            IGNIS_LOG(info) << "IShuffleModule split addr: " << split.addr << ", length: " << split.length;
        }
        IGNIS_LOG(info) << "IShuffleModule finished";
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

void IShuffleModule::joinSplits(const std::vector<int64_t> &order) {
    IGNIS_LOG(info) << "IShuffleModule joining splits";
    try {
        auto object = getIObject(executor_data->loadObject()->getManager());
        auto msgs = executor_data->getPostBox().popInBox();
        for (auto id:order) {
            msgs[id].getObj()->moveTo(*object);
        }
        executor_data->loadObject(object);
        IGNIS_LOG(info) << "IShuffleModule splits joined";
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

IShuffleModule::~IShuffleModule() {
}



