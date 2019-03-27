
#include "IKeysModule.h"
#include <unordered_map>
#include "../ILog.h"
#include "../IMessage.h"
#include "../storage/iterator/IElementIterator.h"
#include "../../../exceptions/IInvalidArgument.h"
#include "../../api/function/IFunction2.h"


using namespace ignis::executor::core;
using namespace ignis::executor::api;
using namespace ignis::executor::core::storage;
using namespace ignis::executor::core::modules;
using ignis::rpc::IRemoteException;

IKeysModule::IKeysModule(std::shared_ptr<IExecutorData> &executor_data) : IgnisModule(executor_data) {}

void IKeysModule::getKeys(std::vector<int64_t> &_return) {
    try {
        IGNIS_LOG(info) << "IKeysModule starting getKeys";
        auto size = executor_data->loadObject()->getSize();
        _return.resize(size);
        memcpy(&_return[0], hashes.get(), size * sizeof(int64_t));
        IGNIS_LOG(info) << "IKeysModule keys ready";
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

void IKeysModule::getKeysWithCount(std::unordered_map<int64_t, int64_t> &_return) {
}

void IKeysModule::collectKeys() {
    try {
        IGNIS_LOG(info) << "IKeysModule collecting keys";
        auto manager = getManager(*executor_data->loadObject());
        auto msgs = executor_data->getPostBox().popInBox();

        auto object_out = getIObject(manager);

        for (auto &entry: msgs) {
            entry.second.getObj()->setManager(manager);
            entry.second.getObj()->moveTo(*object_out);
        }
        executor_data->loadObject(object_out);
        IGNIS_LOG(info) << "IKeysModule keys collected";
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

void IKeysModule::prepareKeys(const std::vector<ignis::rpc::executor::IExecutorKeys> &executorKeys) {
    try {
        IGNIS_LOG(info) << "IKeysModule preparing keys";
        auto object = executor_data->loadObject();
        auto manager = getManager(*object);
        std::unordered_map<int64_t, decltype(object->writeIterator())> hashWriter;
        size_t size = object->getSize();

        for (auto &entry:executorKeys) {
            auto msg_object = getIObject(manager, object->getSize() / executorKeys.size());
            executor_data->getPostBox().newOutMessage(entry.msg_id, IMessage(entry.addr, msg_object));
            auto writer = msg_object->writeIterator();
            for (auto &id:entry.keys) {
                hashWriter[id] = writer;
            }
        }

        auto reader = object->readIterator();
        for (size_t i = 0; i < size; i++) {
            hashWriter[hashes.get()[i]]->write((storage::IObject::Any &&) reader->next());
        }
        hashes.reset();
        IGNIS_LOG(info) << "IKeysModule keys prepared";
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

struct IOperatorLess {
    static std::shared_ptr<ignis::data::handle::IOperator<storage::IObject::Any>> o;

    bool operator()(const std::shared_ptr<storage::IObject::Any> &__x,
                    const std::shared_ptr<storage::IObject::Any> &__y) const {
        return o->less(*__x, *__y);
    }
};

std::shared_ptr<ignis::data::handle::IOperator<storage::IObject::Any>> IOperatorLess::o;

void IKeysModule::reduceByKey(const rpc::ISource &funct) {
    try {
        IGNIS_LOG(info) << "IKeysModule reduceByKey starting";
        typedef api::function::IFunction2<IObject::Any, IObject::Any, IObject::Any> IFunction2_Type;
        auto function = loadSource<IFunction2_Type>(funct);
        auto &context = executor_data->getContext();
        auto object = executor_data->loadObject();
        auto manager = getManager(*object);
        auto pair_manager = (std::shared_ptr<api::IPairManager<storage::IObject::Any, storage::IObject::Any>> &) manager;
        auto op = pair_manager->firstManager()->_operator();
        size_t threads = executor_data->getThreads();
        size_t size = object->getSize();
        hashes = std::shared_ptr<int64_t>(new ssize_t[size], std::default_delete<int64_t[]>());
        size_t n_buckets = sizeof(int64_t) * threads;
        decltype(object) buckets[n_buckets];
        decltype(object->writeIterator()) w_buckets[n_buckets];
        auto reader = object->readIterator();
        //Split keys in buckets using hash
        for (size_t i = 0; i < size; i++) {
            auto &value = reader->next();
            auto hash = op->hash(value);
            hashes.get()[i] = hash;
            auto bucket = hash % n_buckets;
            if (!buckets[bucket]) {
                buckets[bucket] = getIObject(manager, 2 * size / n_buckets);
                w_buckets[bucket] = buckets[bucket]->writeIterator();
            }
            w_buckets[bucket]->write((storage::IObject::Any &&) value);
        }
        object->clear();

        std::vector<std::shared_ptr<storage::IObject>> keys;
        IOperatorLess::o = op;
        typedef std::map<
                std::shared_ptr<storage::IObject::Any>,
                decltype(object->writeIterator()),
                IOperatorLess
        > Map;
//separate different keys inside each bucket
#pragma omp parallel for schedule(dynamic) num_threads(threads)
        for (int i = 0; i < n_buckets; i++) {
            auto &bucket = buckets[i];
            if (bucket && bucket->getSize() > 0) {
                auto size = bucket->getSize();
                auto reader = bucket->readIterator();
                Map writers;
                for (size_t j = 0; j < size; j++) {
                    auto value = reader->nextShared();
                    auto node = writers.lower_bound(value);
                    if (node != writers.end() && !(writers.key_comp()(value, node->first))) {
                        node->second->write((storage::IObject::Any &&) *value);
                    } else {
                        auto key_obj = getIObject(manager);
                        auto writer = key_obj->writeIterator();
                        writers.insert(node, Map::value_type(value, writer));
#pragma omp critical
                        {
                            keys.push_back(key_obj);
                        }
                        writer->write(*value);
                    }
                }
            }
            bucket.reset();
        }

        auto object_out = getIObject(manager, keys.size());
        auto writer_out = object_out->writeIterator();
        typedef typename IPairManager<IFunction2_Type::Any, storage::IObject::Any>::Class F_arg;
        typedef IPairManager<IFunction2_Type::Any, storage::IObject::Any> M_arg;

//Reduce values with same key
        function->before(context);
#pragma omp parallel for schedule(dynamic) num_threads(threads)
        for (int i = 0; i < keys.size(); i++) {
            auto &object_key = keys[i];
            auto reader = object_key->readIterator();
            auto size = object_key->getSize();
            storage::iterator::IElementIterator base(manager);
            base.write((storage::IObject::Any &&) reader->next());
            for (size_t j = 0; j < size - 1; j++) {
                function->writeReduceByKey((F_arg &) reader->next(), (F_arg &) base.next(), context,
                                              (M_arg &) *manager);
            }
#pragma omp critical
            {
                writer_out->write((storage::IObject::Any &&) base.next());
            }
            object_key.reset();
        }
        function->after(context);
        object_out->fit();
        executor_data->loadObject(object_out);
        IGNIS_LOG(info) << "IKeysModule reduceByKey ready";
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

IKeysModule::~IKeysModule() {

}


