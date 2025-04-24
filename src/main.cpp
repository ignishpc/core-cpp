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
    if (argc < 2) {
        IGNIS_LOG(error) << "Executor requires a socket address";
        return EXIT_FAILURE;
    }

    const char* usock =  argv[1];
    int compression = 0;
    try {
        const char* value = std::getenv("IGNIS_TRANSPORT_COMPRESSION");
        if (value){
            compression = std::atoi(value);
        }
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
    server.serve("IExecutorServer", usock, compression);

    return EXIT_SUCCESS;
}
