
#include "IReducerModule.h"
#include "../ILog.h"
#include "../../../exceptions/IInvalidArgument.h"

using namespace std;
using namespace ignis::executor::core::modules;
using namespace ignis::executor::core::storage;
using namespace ignis::executor::core;
using ignis::rpc::IRemoteException;

IReducerModule::IReducerModule(std::shared_ptr<IExecutorData> &executor_data) : IgnisModule(executor_data) {}

void IReducerModule::reduceByKey() {
    try {
        //TODO update
        /*
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
        executor_data->loadObject(object_out);*/
        IGNIS_LOG(info) << "IReducerModule finished";
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

IReducerModule::~IReducerModule() {

}
