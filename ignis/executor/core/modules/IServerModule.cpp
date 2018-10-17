
#include "IServerModule.h"
#include <thrift/transport/TServerSocket.h>
#include <thrift/processor/TMultiplexedProcessor.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/protocol/TCompactProtocol.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/concurrency/PlatformThreadFactory.h>
#include "../ILog.h"

using namespace ignis::executor::core::modules;
using namespace apache::thrift::transport;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using ignis::rpc::IRemoteException;

IServerModule::IServerModule(shared_ptr<IExecutorData> &executor_data) : IgnisModule(executor_data){}

void IServerModule::start(shared_ptr<apache::thrift::TProcessor> processor, int port) {
    if(!server) {
        auto threadManager = ThreadManager::newSimpleThreadManager(1);

        threadManager->threadFactory(std::make_shared<PlatformThreadFactory>());
        threadManager->start();
        server = std::make_shared<apache::thrift::server::TThreadPoolServer>(
                processor,
                std::make_shared<TServerSocket>(port),
                std::make_shared<TBufferedTransportFactory>(),
                std::make_shared<TCompactProtocolFactory>(),
                threadManager
        );
        server->serve();
        server.reset();
        threadManager->stop();
    }
}

void IServerModule::stop() {
    try {
        if(server) {
            server->stop();
        }
    } catch (exceptions::IException &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack(ex.toString());
        throw iex;
    } catch (std::exception &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack("UNKNOWN");
        throw iex;
    }
}

bool IServerModule::test() {
    return true;
}

void IServerModule::updateProperties(const std::map<std::string, std::string> & properties) {
    try {
        executor_data->getContext().getProperties().insert(properties.begin(), properties.end());
    } catch (exceptions::IException &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack(ex.toString());
        throw iex;
    } catch (std::exception &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack("UNKNOWN");
        throw iex;
    }
}

IServerModule::~IServerModule() {

}
