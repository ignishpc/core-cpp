
#include "ICallBack.h"
#include "ignis/executor/core/ILog.h"

#include <thrift/processor/TMultiplexedProcessor.h>
#include "ignis/executor/core/IExecutorData.h"
#include "ignis/executor/core/modules/IExecutorServerModule.h"
#include "ignis/executor/core/modules/IIOModule.h"
#include "ignis/executor/core/modules/ICommModule.h"
#include <thread>


using namespace ignis::executor::core;
using namespace ignis::executor::core::modules;
using namespace ignis::rpc::executor;
using namespace ignis::driver::core;

ICallBack::ICallBack(int port, int compression) {
    IGNIS_LOG_INIT();

    auto processor = std::make_shared<apache::thrift::TMultiplexedProcessor>();
    auto executor_data = std::make_shared<IExecutorData>();

    auto server = std::make_shared<IExecutorServerModule>(executor_data);
    processor->registerProcessor("IExecutorServer", std::make_shared<IExecutorServerModuleProcessor>(server));
    auto io = std::make_shared<IIOModule>(executor_data);
    processor->registerProcessor("IIO", std::make_shared<IIOModuleProcessor>(io));
    driverContext = std::make_shared<IDriverContext>(executor_data);
    processor->registerProcessor("ICacheContex", std::make_shared<ICacheContextModuleProcessor>(driverContext));
    auto comm = std::make_shared<ICommModule>(executor_data);
    processor->registerProcessor("IComm", std::make_shared<ICommModuleProcessor>(comm));

    this->server = server;
    std::thread([&]() { server->start(*processor, port, compression); }).detach();
}

ignis::driver::core::IDriverContext &ICallBack::getDriverContext() {
    return *driverContext;
}

ICallBack::~ICallBack() {
    if (server) {
        server->stop();
    }
}