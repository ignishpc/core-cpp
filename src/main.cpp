#include "ignis/executor/core/ILog.h"

#include <thrift/processor/TMultiplexedProcessor.h>
#include "ignis/executor/core/IExecutorData.h"
#include "ignis/executor/core/modules/IExecutorServerModule.h"
#include "ignis/executor/core/modules/IGeneralModule.h"
#include "ignis/executor/core/modules/IGeneralActionModule.h"
#include "ignis/executor/core/modules/IMathModule.h"
#include "ignis/executor/core/modules/IIOModule.h"
#include "ignis/executor/core/modules/ICacheContextModule.h"
#include "ignis/executor/core/modules/ICommModule.h"


using namespace ignis::executor::core;
using namespace ignis::executor::core::modules;
using namespace ignis::rpc::executor;

int main(int argc, char *argv[]) {
    IGNIS_LOG_INIT();

    auto processor = std::make_shared<apache::thrift::TMultiplexedProcessor>();
    auto executor_data = std::make_shared<IExecutorData>();

    auto server = std::make_shared<IExecutorServerModule>(executor_data);
    processor->registerProcessor("IExecutorServer", std::make_shared<IExecutorServerModuleProcessor>(server));
    auto general = std::make_shared<IGeneralModule>(executor_data);
    processor->registerProcessor("IGeneral", std::make_shared<IGeneralModuleProcessor>(general));
    auto general_action = std::make_shared<IGeneralActionModule>(executor_data);
    processor->registerProcessor("IGeneralAction", std::make_shared<IGeneralActionModuleProcessor>(general_action));
    auto math = std::make_shared<IMathModule>(executor_data);
    processor->registerProcessor("IMath", std::make_shared<IMathModuleProcessor>(math));
    auto io = std::make_shared<IIOModule>(executor_data);
    processor->registerProcessor("IIO", std::make_shared<IIOModuleProcessor>(io));
    auto cache_context = std::make_shared<ICacheContextModule>(executor_data);
    processor->registerProcessor("ICacheContex", std::make_shared<ICacheContextModuleProcessor>(cache_context));
    auto comm = std::make_shared<ICommModule>(executor_data);
    processor->registerProcessor("IComm", std::make_shared<ICommModuleProcessor>(comm));

    if (argc != 2) {
        IGNIS_LOG(error) << "Executor need a server port and compression as argument";
        return EXIT_FAILURE;
    }

    int port = atoi(argv[1]);
    int compression = atoi(argv[2]);
    if (port == 0) {
        IGNIS_LOG(error) << "Executor need a valid server port and compression";
        return EXIT_FAILURE;
    }

    server->start(*processor, port, compression);

    return EXIT_SUCCESS;
}
