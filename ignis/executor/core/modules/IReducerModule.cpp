
#include "IReducerModule.h"
#include "../ILog.h"
#include "../../../exceptions/IInvalidArgument.h"
#include "../../api/function/IPairFunction2.h"

using namespace std;
using namespace ignis::executor::core::modules;
using namespace ignis::executor::core::storage;
using namespace ignis::executor::core;
using ignis::rpc::IRemoteException;

IReducerModule::IReducerModule(std::shared_ptr<IExecutorData> &executor_data) : IgnisModule(executor_data) {}

void IReducerModule::reduceByKey(const rpc::ISourceFunction &sf) {
    IGNIS_LOG(info) << "IReduceModule starting reduce by key";
    try {
        auto function = loadFunction<api::function::IPairFunction2<IObject::Any, IObject::Any, IObject::Any, IObject::Any>>(
                sf);
        auto object_in = executor_data->getSharedLoadObject();
        auto manager_t = (*function)->type_pt1.shared();
        auto manager_r = (*function)->type_pr.shared();
        auto manager_t_any = (std::shared_ptr<data::IManager<IObject::Any>> &) manager_t;
        auto manager_r_any = (std::shared_ptr<data::IManager<IObject::Any>> &) manager_r;
        object_in->setManager(manager_t_any);
        auto object_out = getIObject(manager_r_any);
        auto writer = object_out->writeIterator();
        size_t threads = executor_data->getThreads();
        auto &context = executor_data->getContext();

        std::vector<std::shared_ptr<storage::IObject>> matches;
        keyMatch(matches);
        auto size = matches.size();
        executor_data->loadObject(object_out);


        (*function)->before(executor_data->getContext());
#pragma omp parallel for num_threads(threads)
        for (int i = 0; i < size; i++) {
            auto &match_object = *matches[i];
            auto match_read = match_object.readIterator();
            if (match_object.getSize() == 1) {
#pragma omp critical
                writer->write(match_read->next());
            } else {
                auto object_result = getIObject(manager_t_any, 1, 0, "memory");
                object_result->writeIterator()->write(match_read->next());
                while (match_read->hashNext()) {
                    (*function)->writeCall(
                            (pair<IObject::Any, IObject::Any> &) (object_result->readIterator()->next()),
                            (pair<IObject::Any, IObject::Any> &) (match_read->next()),
                            context,
                            (api::IWriteIterator<pair<IObject::Any, IObject::Any>> &) (*object_result->writeIterator())
                    );
                }
#pragma omp critical
                writer->write(object_result->readIterator()->next());
            }
        }
        (*function)->after(executor_data->getContext());
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

void IReducerModule::groupByKey() {

}


void IReducerModule::keyMatch(std::vector<std::shared_ptr<storage::IObject>> &matches) {
    IGNIS_LOG(info) << "IReduceModule matching keys";
    unordered_map<string, std::pair<std::shared_ptr<storage::IObject>, std::shared_ptr<ICoreWriteIterator<IObject::Any>>>> objects;
    auto object_in = executor_data->getSharedLoadObject();
    auto vector_manager = object_in->getManager()->getClassManagerType();
    auto elem_manager = vector_manager->getElemClassManager();
    auto pair_manager = (data::serialization::IClassManagerType<std::pair<IObject::Any, IObject::Any>> &) (*elem_manager);
    auto first_op = pair_manager.getFirstIOperator();
    auto it = object_in->readIterator();

    while (it->hashNext()) {
        auto &value = it->next();
        auto key = first_op->toString(pair_manager.first((std::pair<IObject::Any, IObject::Any> &) value));
        auto &pair = objects[key];
        if (pair.first) {
            auto object_key = getIObject(object_in->getManager());
            pair.first = object_key;
            pair.second = object_key->writeIterator();
            matches.push_back(object_key);
        }
        if (it->isMoved()) {
            pair.second->write((IObject::Any &&) value);
        } else {
            pair.second->write(value);
        }
    }
    IGNIS_LOG(info) << "IReduceModule keys matched";
}

IReducerModule::~IReducerModule() {

}
