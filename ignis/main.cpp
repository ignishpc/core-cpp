#include "executor/core/ILog.h"

#include <thrift/processor/TMultiplexedProcessor.h>
#include "executor/core/IExecutorData.h"
#include "executor/core/modules/IGeneralModule.h"
#include "executor/core/modules/IGeneralActionModule.h"
/*
#include "executor/core/modules_old/IFilesModule.h"
#include "executor/core/modules_old/IKeysModule.h"
#include "executor/core/modules_old/IMapperModule.h"
#include "executor/core/modules_old/IPostmanModule.h"
#include "executor/core/modules_old/IReducerModule.h"
#include "executor/core/modules_old/IServerModule.h"
#include "executor/core/modules_old/IShuffleModule.h"
#include "executor/core/modules_old/ISortModule.h"
#include "executor/core/modules_old/IStorageModule.h"*/
#include "executor/api/function/IFunction2.h"
#include <vector>


using namespace ignis::executor::core;
using namespace ignis::executor::core::modules;
using namespace ignis::executor::core::transport;
//using namespace ignis::rpc::executor;


class MyFuntion2 : public ignis::executor::api::function::IFunction2<int, int, int> {
};


ignis_export(test, MyFuntion2);

int main(int argc, char *argv[]) {
    IGNIS_LOG_INIT();

    auto processor = std::make_shared<apache::thrift::TMultiplexedProcessor>();
    auto executor_data = std::make_shared<ignis::executor::core::IExecutorData>();


    impl::IReduceImpl reduce(executor_data);
    test_constructor()->general_action->reduce(reduce);

    exit(0);


    auto general = std::make_shared<ignis::executor::core::modules::IGeneralModule>(executor_data);
    /*processor->registerProcessor("general", std::make_shared<IMapperModuleProcessor>(general));
    auto files = std::make_shared<IFilesModule>(executor_data);
    processor->registerProcessor("files", std::make_shared<IFilesModuleProcessor>(files));
    auto keys = std::make_shared<IKeysModule>(executor_data);
    processor->registerProcessor("keys", std::make_shared<IKeysModuleProcessor>(keys));
    auto mapper = std::make_shared<IMapperModule>(executor_data);
    processor->registerProcessor("mapper", std::make_shared<IMapperModuleProcessor>(mapper));
    auto postman = std::make_shared<IPostmanModule>(executor_data);
    processor->registerProcessor("postman", std::make_shared<IPostmanModuleProcessor>(postman));
    auto reducer = std::make_shared<IReducerModule>(executor_data);
    processor->registerProcessor("reducer", std::make_shared<IReducerModuleProcessor>(reducer));
    auto server = std::make_shared<IServerModule>(executor_data);
    processor->registerProcessor("server", std::make_shared<IServerModuleProcessor>(server));
    auto shuffle = std::make_shared<IShuffleModule>(executor_data);
    processor->registerProcessor("shuffle", std::make_shared<IShuffleModuleProcessor>(shuffle));;
    auto sort = std::make_shared<ISortModule>(executor_data);
    processor->registerProcessor("sort", std::make_shared<ISortModuleProcessor>(sort));;
    auto storage_old = std::make_shared<IStorageModule>(executor_data);
    processor->registerProcessor("storage_old", std::make_shared<IStorageModuleProcessor>(storage_old));*/

    if (argc == 1) {
        IGNIS_LOG(error) << "Executor need a server port as argument";
        return EXIT_FAILURE;
    }

    int port = atoi(argv[1]);
    if (port == 0) {
        IGNIS_LOG(error) << "Executor need a valid server port as argument";
        return EXIT_FAILURE;
    }

    //server->start(processor, port);

    return EXIT_SUCCESS;
}
