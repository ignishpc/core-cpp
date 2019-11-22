#include "ignis/executor/core/ILog.h"

#include <thrift/processor/TMultiplexedProcessor.h>
#include "ignis/executor/core/IExecutorData.h"
#include "ignis/executor/core/modules/IExecutorServerModule.h"
#include "ignis/executor/core/modules/IGeneralActionModule.h"
#include "ignis/executor/core/modules/IGeneralModule.h"
#include "ignis/executor/core/modules/IIOModule.h"


using namespace ignis::executor::core;
using namespace ignis::executor::core::modules;
using namespace ignis::rpc::executor;

int main(int argc, char *argv[]) {
    IGNIS_LOG_INIT();

    auto processor = std::make_shared<apache::thrift::TMultiplexedProcessor>();
    auto executor_data = std::make_shared<IExecutorData>();

    auto server = std::make_shared<IExecutorServerModule>(executor_data);
    processor->registerProcessor("server", std::make_shared<IExecutorServerModuleProcessor>(server));
    auto general_action = std::make_shared<IGeneralActionModule>(executor_data);
    processor->registerProcessor("general", std::make_shared<IGeneralActionModuleProcessor>(general_action));
    auto general = std::make_shared<IGeneralModule>(executor_data);
    processor->registerProcessor("general", std::make_shared<IGeneralModuleProcessor>(general));
    auto io = std::make_shared<IIOModule>(executor_data);
    processor->registerProcessor("io", std::make_shared<IIOModuleProcessor>(io));

    if (argc == 1) {
        IGNIS_LOG(error) << "Executor need a server port as argument";
        return EXIT_FAILURE;
    }

    int port = atoi(argv[1]);
    if (port == 0) {
        IGNIS_LOG(error) << "Executor need a valid server port as argument";
        return EXIT_FAILURE;
    }

    server->start(*processor, port);

    return EXIT_SUCCESS;
}
