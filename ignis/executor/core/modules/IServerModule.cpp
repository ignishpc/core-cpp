
#include "IServerModule.h"
#include <thrift/transport/TServerSocket.h>
#include <thrift/processor/TMultiplexedProcessor.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/protocol/TCompactProtocol.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/concurrency/PlatformThreadFactory.h>

using namespace std;
using namespace ignis::executor::core::modules;
using namespace apache::thrift::transport;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;

IServerModule::IServerModule(shared_ptr<IExecutorData> &executor_data, int port) : IgnisModule(executor_data),
                                                                                   port(port), server_ptr(NULL) {}


void IServerModule::start(shared_ptr<apache::thrift::TProcessor> &processor) {
    auto threadManager = ThreadManager::newSimpleThreadManager(1);

    threadManager->threadFactory(make_shared<PlatformThreadFactory>());
    threadManager->start();
    auto server = make_shared<apache::thrift::server::TThreadPoolServer>(
            processor,
            make_shared<TServerSocket>(port),
            make_shared<TBufferedTransportFactory>(),
            make_shared<TCompactProtocolFactory>(),
            threadManager
    );
    server_ptr = server.get();
    server->serve();
    server_ptr = NULL;
    threadManager->stop();
}


void IServerModule::stop() {
    if(server_ptr != NULL){
        server_ptr->stop();
    }
}

bool IServerModule::test(){
    return true;
}

void IServerModule::setProperties(const std::map<std::string, std::string> & properties){
    executor_data->getContext().getProperties().insert(properties.begin(), properties.end());
}
