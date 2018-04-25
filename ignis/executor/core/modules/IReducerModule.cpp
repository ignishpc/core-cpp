
#include "IReducerModule.h"
#include "../ILog.h"
#include "../../../exceptions/IInvalidArgument.h"
#include "../storage/IMessage.h"
#include <map>

using namespace ignis::executor::core;
using namespace ignis::executor::core::storage;
using namespace ignis::executor::core::modules;
using ignis::rpc::executor::IFunction;
using ignis::rpc::IRemoteException;

IReducerModule::IReducerModule(std::shared_ptr<IExecutorData> &executor_data) : IgnisModule(executor_data) {}

void IReducerModule::getKeys(std::unordered_map<int64_t, int64_t> &_return, const rpc::executor::IFunction &funct,
                             const bool single) {
    try {
        IGNIS_LOG(info) << "IReducerModule starting reducer";
        function = loadFunction<api::IReducer<IObject::Any, IObject::Any, IObject::Any>>(funct);
        std::unordered_map<int64_t, int64_t> keys_count;
        std::unordered_map<int64_t, std::shared_ptr<ICoreWriteIterator<IObject::Any>>> keys_write;
        auto &object_in = executor_data->getLoadObject();
        auto it = object_in.readIterator();
        auto type_k = (*function)->type_k.shared();
        manager = (std::shared_ptr<data::IManager<IObject::Any >> &) type_k;
        auto ops = (*function)->op_t.shared();
        IGNIS_LOG(info) << "IReducerModule separating keys";
        while (it->hashNext()) {
            auto &value = it->next();
            auto key_id = ops->hash(value);
            if (keys_count.find(key_id) == keys_count.end()) {
                auto st = getIObject(manager);
                hash_value[key_id] = st;
                keys_write[key_id] = st->writeIterator();
            }
            if (it->isMoved()) {
                keys_write[key_id]->write((IObject::Any &&) value);
            } else {
                keys_write[key_id]->write(value);
            }
            keys_count[key_id]++;
        }
        if (!single) {
            _return = std::move(keys_count);
        }
        IGNIS_LOG(info) << "IReducerModule keys ready";
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

void IReducerModule::setExecutorKeys(const std::string &host, const int32_t port, const std::vector<int64_t> &keys_id,
                                     const int64_t msg_id) {
    try {
        IGNIS_LOG(info) << "IReducerModule starting key swap";
        auto msg_obj = getIObject();
        msg_obj->setManager(manager);
        IMessage msg(host, port, msg_obj);
        auto writer = msg_obj->writeIterator();
        for (auto &id : keys_id) {
            readToWrite<IObject::Any>(*hash_value[id]->readIterator(), *writer, true);
            hash_value.erase(id);
        }
        executor_data->getPostBox().newMessage(msg_id, msg);
        IGNIS_LOG(info) << "IReducerModule key swap prepared";
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

void IReducerModule::joinDataAux(IObject &obj, std::unordered_map<int64_t, std::vector<std::pair<
        std::shared_ptr<storage::IObject>, std::shared_ptr<storage::ICoreWriteIterator<storage::IObject::Any>>>>> &keys) {
    auto ops = (*function)->op_t.shared();
    auto it = obj.readIterator();
    while (it->hashNext()) {
        auto &value = it->next();
        auto hash = ops->hash(value);
        auto &buckets = keys[ops->hash(value)];
        bool find = false;
        for (auto &key_obj : buckets) {
            auto key_read = key_obj.first->readIterator();
            if (ops->equals(key_read->next(), value)) {
                key_obj.second->write((IObject::Any &&) value);
                find = true;
            }
        }
        if (!find) {
            auto key_obj = getIObject(manager);
            auto writer = key_obj->writeIterator();
            writer->write((IObject::Any &&) value);
            buckets.push_back(
                    std::pair<std::shared_ptr<IObject>, std::shared_ptr<ICoreWriteIterator<IObject::Any>>>
                            (key_obj, writer));
        }
    }
}

void IReducerModule::joinData(const std::vector<int64_t> &msg_ids) {
    try {
        std::unordered_map<int64_t, std::vector<std::pair<
                std::shared_ptr<storage::IObject>, std::shared_ptr<storage::ICoreWriteIterator<storage::IObject::Any>>>>> keys;
        IGNIS_LOG(info) << "IReducerModule joining keys";
        auto ops = (*function)->op_t.shared();
        auto msgs = executor_data->getPostBox().getMessages();
        key_value.reserve(msg_ids.size() + hash_value.size());
        for (auto &id : msg_ids) {
            auto msg_obj = msgs[id].getObj();
            msg_obj->setManager(manager);
            joinDataAux(*msg_obj, keys);
            msgs.erase(id);
        }
        for (auto &elem: hash_value) {
            joinDataAux(*elem.second, keys);
        }
        hash_value.clear();

        for (auto &vector_pairs: keys) {
            for (auto &pair: vector_pairs.second) {
                key_value.push_back(pair.first);
            }
        }
        IGNIS_LOG(info) << "IReducerModule keys joined";
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

void IReducerModule::reduceByKey() {
    try {
        auto object_out = getIObject(manager);
        size_t threads = executor_data->getThreads();
        auto writer = object_out->writeIterator();
        (*function)->before(executor_data->getContext());
        if (threads > 1) {
            IGNIS_LOG(info) << "IReducerModule creating " << threads << " threads";
#pragma omp parallel for num_threads(threads) ordered
            for (int i = 0; i < key_value.size(); i++) {
                auto &obj = key_value[i];
                auto object_out_aux = getIObject(manager);
                auto thread_it_r = obj->readIterator();
                auto &it_pair = (ICoreReadIterator<std::pair<IObject::Any, IObject::Any>> &) (*thread_it_r);
                auto thread_it_w = object_out_aux->writeIterator();
                (*function)->reduce(it_pair, *thread_it_w, executor_data->getContext());
#pragma omp ordered
                readToWrite<IObject::Any>(*object_out_aux->readIterator(), *writer, true);
            }
        } else {
            for (int i = 0; i < key_value.size(); i++) {
                auto &obj = key_value[i];
                auto it_r = obj->readIterator();
                auto &it_pair = (ICoreReadIterator<std::pair<IObject::Any, IObject::Any>> &) (*it_r);
                (*function)->reduce(it_pair, *writer, executor_data->getContext());
            }
        }
        (*function)->after(executor_data->getContext());
        object_out->fit();
        executor_data->loadObject(object_out);
        IGNIS_LOG(info) << "IReducerModule finished";
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

void IReducerModule::reset() {
    try {
        manager.reset();
        function.reset();
        hash_value.clear();
        hash_value.reserve(1);
        key_value.clear();
        key_value.reserve(1);
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

IReducerModule::~IReducerModule() {

}
