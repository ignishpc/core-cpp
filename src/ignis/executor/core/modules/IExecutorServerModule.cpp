
#include "IExecutorServerModule.h"
#include <thrift/transport/TServerSocket.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/protocol/TCompactProtocol.h>
#include "ignis/executor/core/transport/IZlibTransport.h"
#include <thrift/concurrency/ThreadFactory.h>

using namespace ignis::executor::core::modules;
using namespace apache::thrift;

IExecutorServerModule::IExecutorServerModule(std::shared_ptr<IExecutorData> &executor_data) : IModule(executor_data) {}

IExecutorServerModule::~IExecutorServerModule() {}

void IExecutorServerModule::start(TProcessor &procesor, int port, int compression) {
    if (!server) {
        auto threadManager = concurrency::ThreadManager::newSimpleThreadManager(1);

        threadManager->threadFactory(std::make_shared<concurrency::ThreadFactory>());
        threadManager->start();

        server = std::make_shared<apache::thrift::server::TThreadPoolServer>(
                std::shared_ptr<apache::thrift::TProcessor>(&procesor, [](apache::thrift::TProcessor *) {}),
                std::make_shared<apache::thrift::transport::TServerSocket>(port),
                std::make_shared<transport::IZlibTransportFactory>(compression),
                std::make_shared<apache::thrift::protocol::TCompactProtocolFactory>(),
                threadManager
        );
        server->serve();
        server.reset();
        threadManager->stop();
    }
}

void IExecutorServerModule::updateProperties(const std::map<std::string, std::string> &properties) {
    IGNIS_RPC_TRY()
        executor_data->getContext().props().insert(properties.begin(), properties.end());
        executor_data->setCores(executor_data->getProperties().cores());
    IGNIS_RPC_CATCH()
}

void IExecutorServerModule::stop() {
    IGNIS_RPC_TRY()
        if (server) {
            server->stop();
        }
    IGNIS_RPC_CATCH()
}

bool IExecutorServerModule::test() {
    return true;
}
