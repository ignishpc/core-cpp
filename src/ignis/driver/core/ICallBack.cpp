
#include "ICallBack.h"
#include "ignis/executor/core/ILog.h"

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

    class IExecutorServerModuleImpl : public IExecutorServerModule {
    public:
        IExecutorServerModuleImpl(std::shared_ptr<IExecutorData> &executor_data,
                                  std::shared_ptr<core::IDriverContext> &driverContext) :
                IExecutorServerModule(executor_data), driverContext(driverContext) {}

        void createServices(apache::thrift::TMultiplexedProcessor &processor) {
            auto io = std::make_shared<IIOModule>(executor_data);
            processor.registerProcessor("IIO", std::make_shared<IIOModuleProcessor>(io));
            processor.registerProcessor("ICacheContex", std::make_shared<ICacheContextModuleProcessor>(driverContext));
            auto comm = std::make_shared<ICommModule>(executor_data);
            processor.registerProcessor("IComm", std::make_shared<ICommModuleProcessor>(comm));
        }

        std::shared_ptr<core::IDriverContext> driverContext;
    };

    auto executor_data = std::make_shared<IExecutorData>();
    driverContext = std::make_shared<IDriverContext>(executor_data);
    auto server = std::make_shared<IExecutorServerModuleImpl>(executor_data, driverContext);
    std::thread([&]() { server->serve("IExecutorServer", port, compression); }).detach();
    this->server = server;
}

ignis::driver::core::IDriverContext &ICallBack::getDriverContext() {
    return *driverContext;
}

ICallBack::~ICallBack() {
    if (server) {
        server->stop();
    }
}