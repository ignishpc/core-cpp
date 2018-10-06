
#include "IKeysModule.h"
#include "../ILog.h"
#include "../../../exceptions/IInvalidArgument.h"
#include "../storage/IMessage.h"
#include "../storage/iterator/IFilterIterator.h"
#include "../storage/IObjectWrapperIterator.h"
#include <unordered_set>

using namespace ignis::executor::core;
using namespace ignis::executor::core::storage;
using namespace ignis::executor::core::modules;
using ignis::rpc::IRemoteException;

IKeysModule::IKeysModule(std::shared_ptr<IExecutorData> &executor_data) : IgnisModule(executor_data) {}

void IKeysModule::getKeys(std::unordered_map<int64_t, int64_t> &_return, const bool single) {
    try {
        IGNIS_LOG(info) << "IKeysModule starting getKeys";
        auto &object_in = executor_data->getLoadObject();
        auto it = object_in.readIterator();
        auto vector_manager = object_in.getManager()->getClassManagerType();
        auto elem_manager = vector_manager->getElemClassManager();
        auto pair_manager = (data::serialization::IClassManagerType<std::pair<IObject::Any, IObject::Any>> &) (*elem_manager);
        auto first_op = pair_manager.getFirstIOperator();

        while (it->hashNext()) {
            auto &value = it->next();
            auto key_id = first_op->hash(pair_manager.first((std::pair<IObject::Any, IObject::Any> &) value));
            if (_return.find(key_id) == _return.end()) {
                _return[key_id]++;
            } else {
                _return[key_id] = 1;
            }
        }
        counts = _return;
        IGNIS_LOG(info) << "IKeysModule keys ready";
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

class IReadFilterHashesIterator : public iterator::IReadFilterIterator {
public:
    IReadFilterHashesIterator(const std::shared_ptr<ICoreReadIterator<IObject::Any>> &it,
                              const std::shared_ptr<ignis::data::IOperator<IObject::Any>> &op,
                              const std::vector<int64_t> &hashes)
            : IReadFilterIterator(it, op), hashes(std::unordered_set<int64_t>(hashes.begin(), hashes.end())) {}

private:
    bool filter() override {
        auto hash = op->hash(*next_elem);
        return hashes.find(hash) == hashes.end();
    }

    std::unordered_set<int64_t> hashes;

};

void IKeysModule::sendPairs(const std::string& addr, const std::vector<int64_t> & keys_id) {
    try {
        IGNIS_LOG(info) << "IKeysModule starting keys swap";

        auto &object_in = executor_data->getLoadObject();
        auto it = object_in.readIterator();
        auto vector_manager = object_in.getManager()->getClassManagerType();
        auto elem_manager = vector_manager->getElemClassManager();
        auto pair_manager = (data::serialization::IClassManagerType<std::pair<IObject::Any, IObject::Any>> &) (*elem_manager);
        auto first_op = pair_manager.getFirstIOperator();
        size_t count = 0;

        for (auto &id : keys_id) {
            count += counts[id];
            counts.erase(id);
        }
        IGNIS_LOG(info) << "IKeysModule separating keys by executor";

        auto msg_obj = std::make_shared<IObjectWrapperIterator>(std::make_shared<IReadFilterHashesIterator>(
                object_in.readIterator(), first_op, keys_id
        ), count);
        msg_obj->setManager(object_in.getManager());
        IMessage msg(addr, msg_obj);
        executor_data->getPostBox().newMessage(executor_data->getExecutorId(), msg);

        IGNIS_LOG(info) << "IKeysModule keys swap ready";
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

void IKeysModule::joinPairs() {
    try {
        IGNIS_LOG(info) << "IKeysModule starting keys join";

        auto &object_in = executor_data->getLoadObject();
        auto it = object_in.readIterator();
        auto vector_manager = object_in.getManager()->getClassManagerType();
        auto elem_manager = vector_manager->getElemClassManager();
        auto pair_manager = (data::serialization::IClassManagerType<std::pair<IObject::Any, IObject::Any>> &) (*elem_manager);
        auto first_op = pair_manager.getFirstIOperator();
        size_t count = 0;
        std::vector<int64_t> keys_id;

        for (auto &id_count : counts) {
            count += id_count.second;
            keys_id.push_back(id_count.first);
        }

        auto local_keys = std::make_shared<IObjectWrapperIterator>(std::make_shared<IReadFilterHashesIterator>(
                object_in.readIterator(), first_op, keys_id
        ), count);

        IGNIS_LOG(info) << "IKeysModule joining keys";

        auto msgs = executor_data->getPostBox().getMessages();

        std::shared_ptr<IObject> object = getIObject();
        object->setManager(object_in.getManager());
        auto writer = object->writeIterator();

        iterator::readToWrite<IObject::Any>(*(local_keys->readIterator()), *writer, true);

        executor_data->loadObject(object);

        IGNIS_LOG(info) << "IKeysModule keys joined";
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

void IKeysModule::reset() {
    counts.clear();
}

IKeysModule::~IKeysModule() {

}
