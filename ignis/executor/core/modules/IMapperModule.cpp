
#include "IMapperModule.h"
#include "../ILog.h"
#include "../storage/IRawMemoryObject.h"
#include "../storage/IMemoryObject.h"
#include "../../../exceptions/IInvalidArgument.h"
#include "../IDinamicObject.h"
#include "../../api/IMapper.h"

using namespace ignis::executor::core::modules;
using namespace ignis::executor::core::storage;
using namespace ignis::executor::core;
using ignis::rpc::executor::IFunction;
using ignis::rpc::IRemoteException;

IMapperModule::IMapperModule(std::shared_ptr<IExecutorData> &executor_data) : IgnisModule(executor_data) {}

void IMapperModule::onemap(const IFunction &funct) {
    try {
    IDinamicObject<api::IMapper<IObject::Any,IObject::Any>>(funct.name, funct.__isset.bytes);



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

void IMapperModule::flatmap(const IFunction &funct) {
    try {
        IDinamicObject<api::IMapper<IObject::Any,IObject::Any>>(funct.name, funct.__isset.bytes);


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

IMapperModule::~IMapperModule() {

}
