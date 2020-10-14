
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

void IExecutorServerModule::serve(const std::string &name, int port, int compression) {
    if (!server) {
        auto threadManager = concurrency::ThreadManager::newSimpleThreadManager(1);

        threadManager->threadFactory(std::make_shared<concurrency::ThreadFactory>());
        threadManager->start();

        server = std::make_shared<apache::thrift::server::TThreadPoolServer>(
                processor = std::make_shared<apache::thrift::TMultiplexedProcessor>(),
                std::make_shared<apache::thrift::transport::TServerSocket>(port),
                std::make_shared<transport::IZlibTransportFactory>(compression),
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

void IExecutorServerModule::start(const std::map<std::string, std::string> &properties) {
    IGNIS_RPC_TRY()
    executor_data->getContext().props().insert(properties.begin(), properties.end());
    executor_data->setCores(executor_data->getProperties().cores());
    createServices(*processor);
    IGNIS_LOG(info) << "ServerModule: cpp executor ready";
    IGNIS_RPC_CATCH()
}

void IExecutorServerModule::createServices(TMultiplexedProcessor &processor) {}

void IExecutorServerModule::stop() {
    IGNIS_RPC_TRY()
    if (server) { server->stop(); }
    IGNIS_RPC_CATCH()
}

bool IExecutorServerModule::test() {
    IGNIS_LOG(info) << "ServerModule: test ok";
    return true;
}
