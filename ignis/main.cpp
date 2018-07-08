#include "executor/core/ILog.h"

#include <thrift/processor/TMultiplexedProcessor.h>
#include "executor/core/IExecutorData.h"
#include "executor/core/modules/IFilesModule.h"
#include "executor/core/modules/IMapperModule.h"
#include "executor/core/modules/IPostmanModule.h"
#include "executor/core/modules/IReducerModule.h"
#include "executor/core/modules/IServerModule.h"
#include "executor/core/modules/ISortModule.h"
#include "executor/core/modules/IStorageModule.h"
#include "executor/api/function/IFlatMapFunction.h"
#include <set>
using namespace ignis::executor::core::modules;
using namespace ignis::rpc::executor;

int main(int argc, char *argv[]) {
    IGNIS_LOG_INIT();


    ignis::executor::api::function::IFlatMapFunction<int,std::set<int>> a;


    auto processor = std::make_shared<apache::thrift::TMultiplexedProcessor>();
    auto executor_data = std::make_shared<ignis::executor::core::IExecutorData>();

    auto files = std::make_shared<IFilesModule>(executor_data);
    processor->registerProcessor("files", std::make_shared<IFilesModuleProcessor>(files));
    auto mapper = std::make_shared<IMapperModule>(executor_data);
    processor->registerProcessor("mapper", std::make_shared<IMapperModuleProcessor>(mapper));
    auto postman = std::make_shared<IPostmanModule>(executor_data);
    processor->registerProcessor("postman", std::make_shared<IPostmanModuleProcessor>(postman));
    auto reducer = std::make_shared<IReducerModule>(executor_data);
    processor->registerProcessor("reducer", std::make_shared<IReducerModuleProcessor>(reducer));
    auto server = std::make_shared<IServerModule>(executor_data);
    processor->registerProcessor("server", std::make_shared<IServerModuleProcessor>(server));
    auto sort = std::make_shared<ISortModule>(executor_data);
    processor->registerProcessor("sort", std::make_shared<ISortModuleProcessor>(sort));
    auto storage = std::make_shared<IStorageModule>(executor_data);
    processor->registerProcessor("storage", std::make_shared<IStorageModuleProcessor>(storage));

    if (argc == 1) {
        IGNIS_LOG(error) << "Executor need a server port as argument";
        return EXIT_FAILURE;
    }

    int port = atoi(argv[1]);
    if (port == 0) {
        IGNIS_LOG(error) << "Executor need a valid server port as argument";
        return EXIT_FAILURE;
    }

    server->start(processor, port);

    return EXIT_SUCCESS;
}
