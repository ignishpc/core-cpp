
#include "IKeysModule.h"
#include "../ILog.h"
#include "../../../exceptions/IInvalidArgument.h"
#include "../storage/IMessage.h"
#include "../storage/IFilterIterator.h"
#include "../storage/IObjectWrapperIterator.h"
#include <unordered_set>

using namespace std;
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

class IReadFilterHashesIterator : public IReadFilterIterator {
public:
    IReadFilterHashesIterator(const shared_ptr<ICoreReadIterator<IObject::Any>> &it,
                              const shared_ptr<ignis::data::IOperator<IObject::Any>> &op,
                              const std::vector<int64_t> &hashes)
            : IReadFilterIterator(it, op), hashes(unordered_set<int64_t>(hashes.begin(), hashes.end())) {}

private:
    bool filter() override {
        auto hash = op->hash(*next_elem);
        return hashes.find(hash) == hashes.end();
    }

    unordered_set<int64_t> hashes;

};

void IKeysModule::sendPairs(const std::string &host, const int32_t port, const std::vector<int64_t> &keys_id,
                            const int64_t msg_id) {
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

        auto msg_obj = make_shared<IObjectWrapperIterator>(make_shared<IReadFilterHashesIterator>(
                object_in.readIterator(), first_op, keys_id
        ), count);
        msg_obj->setManager(object_in.getManager());
        IMessage msg(host, port, msg_obj);
        executor_data->getPostBox().newMessage(msg_id, msg);

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

void IKeysModule::joinPairs(const std::vector<int64_t> &msg_ids) {
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
            count += id_count.second ;
            keys_id.push_back(id_count.first);
        }

        auto local_keys = make_shared<IObjectWrapperIterator>(make_shared<IReadFilterHashesIterator>(
                object_in.readIterator(), first_op, keys_id
        ), count);

        IGNIS_LOG(info) << "IKeysModule joining keys";

        auto msgs = executor_data->getPostBox().getMessages();

        shared_ptr<IObject> object = getIObject();
        object->setManager(object_in.getManager());
        auto writer = object->writeIterator();

        readToWrite<IObject::Any>(*(local_keys->readIterator()), *writer, true);

        for (auto &id : msg_ids) {
            readToWrite<IObject::Any>(*(msgs[id].getObj()->readIterator()), *writer, true);
        }

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
