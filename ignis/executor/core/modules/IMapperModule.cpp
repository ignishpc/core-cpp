
#include "IMapperModule.h"
#include "../ILog.h"
#include "../storage/IRawMemoryObject.h"
#include "../storage/IMemoryObject.h"
#include "../storage/IThreadSplitter.h"
#include "../../../exceptions/IInvalidArgument.h"
#include "../IDinamicObject.h"
#include "../../api/IMapper.h"

using namespace std;
using namespace ignis::executor::core::modules;
using namespace ignis::executor::core::storage;
using namespace ignis::executor::core;
using ignis::rpc::executor::IFunction;
using ignis::rpc::IRemoteException;

IMapperModule::IMapperModule(std::shared_ptr<IExecutorData> &executor_data) : IgnisModule(executor_data) {}

void IMapperModule::map(const IFunction &funct, bool ordered) {
    try {
        IDinamicObject<api::IMapper<IObject::Any, IObject::Any>> mapper(funct.name, funct.__isset.bytes);
        IObject &object_in = executor_data->getLoadObject();
        auto manager = mapper->type_t.shared();
        object_in.setManager((std::shared_ptr<data::IManager<IObject::Any>> &) manager);
        shared_ptr<IObject> object_out;
        size_t threads = executor_data->getThreads();
        if (threads > 1) {
            auto buffer = executor_data->getParser().getNumber("ignis.executor.cores.buffer");
            IThreadSplitter splitter(object_in, *object_out, threads, buffer, ordered);
#pragma omp parallel for num_threads(threads)
            for (int t = 0; t < threads; t++) {
                auto it = splitter[t];
                mapper->map(*it.first, *it.second, executor_data->getContext());
            }
        } else {
            mapper->map(*object_in.readIterator(), *object_out->writeIterator(), executor_data->getContext());
        }
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

void IMapperModule::orderedMap(const IFunction &funct) {
    map(funct, true);
}

void IMapperModule::unorderedMap(const IFunction &funct) {
    map(funct, false);
}

IMapperModule::~IMapperModule() {

}
