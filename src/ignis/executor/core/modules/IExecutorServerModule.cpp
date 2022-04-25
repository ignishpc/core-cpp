
#include "IExecutorServerModule.h"
#include "ignis/executor/core/transport/IZlibTransport.h"
#include <thrift/concurrency/ThreadFactory.h>
#include <thrift/protocol/TCompactProtocol.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/transport/TServerSocket.h>

using namespace ignis::executor::core::modules;
using namespace apache::thrift;

IExecutorServerModule::IExecutorServerModule(std::shared_ptr<IExecutorData> &executor_data) : IModule(executor_data) {}

IExecutorServerModule::~IExecutorServerModule() {}

void IExecutorServerModule::serve(const std::string &name, int port, int compression, bool local_mode) {
    if (!server) {
        auto threadManager = concurrency::ThreadManager::newSimpleThreadManager(2);

        threadManager->threadFactory(std::make_shared<concurrency::ThreadFactory>());
        threadManager->start();

        server = std::make_shared<apache::thrift::server::TThreadPoolServer>(
                processor = std::make_shared<apache::thrift::TMultiplexedProcessor>(),
                std::make_shared<apache::thrift::transport::TServerSocket>(local_mode ? "127.0.0.1" : "0.0.0.0", port),
                std::make_shared<transport::TZlibTransportFactoryExt>(compression),
                std::make_shared<apache::thrift::protocol::TCompactProtocolFactory>(), threadManager);
        std::shared_ptr<IExecutorServerModule> this_shared(this, [](IExecutorServerModule *) {});
        processor->registerProcessor(name,
                                     std::make_shared<rpc::executor::IExecutorServerModuleProcessor>(this_shared));
        IGNIS_LOG(info) << "ServerModule: cpp executor started";
        server->serve();
        IGNIS_LOG(info) << "ServerModule: cpp executor stopped";
        server.reset();
        threadManager->stop();
    }
}

void IExecutorServerModule::start(const std::map<std::string, std::string> &properties,
                                  const std::map<std::string, std::string> &env) {
    IGNIS_RPC_TRY()
    executor_data->getContext().props().insert(properties.begin(), properties.end());
    executor_data->setCores(executor_data->getProperties().cores());

    for (auto &entry : env) { setenv(entry.first.c_str(), entry.second.c_str(), 1); }

    if (std::getenv("MPI_THREAD_MULTIPLE") != nullptr) {
        MPI::Init_thread(MPI_THREAD_MULTIPLE);
        IGNIS_LOG(info) << "ServerModule: Mpi started in thread mode";
    } else {
        MPI::Init();
        IGNIS_LOG(info) << "ServerModule: Mpi started";
    }

    createServices(*processor);
    IGNIS_LOG(info) << "ServerModule: cpp executor ready";
    IGNIS_RPC_CATCH()
}

void IExecutorServerModule::createServices(TMultiplexedProcessor &processor) {}

void IExecutorServerModule::stop() {
    IGNIS_RPC_TRY()
    MPI::Finalize();
    if (server) { server->stop(); }
    IGNIS_RPC_CATCH()
}

bool IExecutorServerModule::test() {
    IGNIS_LOG(info) << "ServerModule: test ok";
    return true;
}
