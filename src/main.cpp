#include "ignis/executor/core/ILog.h"

#include "ignis/executor/core/ICoreTypes.h"
#include "ignis/executor/core/IExecutorData.h"
#include "ignis/executor/core/modules/ICacheContextModule.h"
#include "ignis/executor/core/modules/ICommModule.h"
#include "ignis/executor/core/modules/IExecutorServerModule.h"
#include "ignis/executor/core/modules/IGeneralActionModule.h"
#include "ignis/executor/core/modules/IGeneralModule.h"
#include "ignis/executor/core/modules/IIOModule.h"
#include "ignis/executor/core/modules/IMathModule.h"


using namespace ignis::executor::core;
using namespace ignis::executor::core::modules;
using namespace ignis::rpc::executor;

int main(int argc, char *argv[]) {
    IGNIS_LOG_INIT();
    if (argc < 4) {
        IGNIS_LOG(error) << "Executor need a server port, compression and server mode as argument";
        return EXIT_FAILURE;
    }

    int port, compression;
    bool local_mode;
    try {
        port = std::atoi(argv[1]);
        compression = std::atoi(argv[2]);
        local_mode = std::atoi(argv[3]) == 1;
    } catch (...) {
        IGNIS_LOG(error) << "Executor arguments are not valid";
        return EXIT_FAILURE;
    }

    class IExecutorServerModuleImpl : public IExecutorServerModule {
    public:
        using IExecutorServerModule::IExecutorServerModule;

        void createServices(apache::thrift::TMultiplexedProcessor &processor) {
            auto general = std::make_shared<IGeneralModule>(executor_data);
            processor.registerProcessor("IGeneral", std::make_shared<IGeneralModuleProcessor>(general));
            auto general_action = std::make_shared<IGeneralActionModule>(executor_data);
            processor.registerProcessor("IGeneralAction",
                                        std::make_shared<IGeneralActionModuleProcessor>(general_action));
            auto math = std::make_shared<IMathModule>(executor_data);
            processor.registerProcessor("IMath", std::make_shared<IMathModuleProcessor>(math));
            auto io = std::make_shared<IIOModule>(executor_data);
            processor.registerProcessor("IIO", std::make_shared<IIOModuleProcessor>(io));
            auto cache_context = std::make_shared<ICacheContextModule>(executor_data);
            processor.registerProcessor("ICacheContext", std::make_shared<ICacheContextModuleProcessor>(cache_context));
            auto comm = std::make_shared<ICommModule>(executor_data);
            processor.registerProcessor("IComm", std::make_shared<ICommModuleProcessor>(comm));

            for (auto &dtype : ICoreTypes::defaultTypes()) { executor_data->registerType(dtype); }

            for (auto &df : ICoreTypes::defaultFunctions()) { executor_data->registerFunction(df); }
        }
    };

    auto executor_data = std::make_shared<IExecutorData>();
    IExecutorServerModuleImpl server(executor_data);
    server.serve("IExecutorServer", port, compression, local_mode);

    return EXIT_SUCCESS;
}
