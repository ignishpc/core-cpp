
#include "IServerModule.h"
#include <thrift/transport/TServerSocket.h>
#include <thrift/processor/TMultiplexedProcessor.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/protocol/TCompactProtocol.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/concurrency/PlatformThreadFactory.h>
#include "../ILog.h"

using namespace std;
using namespace ignis::executor::core::modules;
using namespace apache::thrift::transport;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using ignis::rpc::IRemoteException;

IServerModule::IServerModule(shared_ptr<IExecutorData> &executor_data) : IgnisModule(executor_data),
                                                                         server_ptr(NULL) {}

void IServerModule::start(shared_ptr<apache::thrift::TProcessor> processor, int port) {
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
    try {
        if (server_ptr != NULL) {
            server_ptr->stop();
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

void IServerModule::setContext(const int64_t id, const std::map<std::string, std::string> &properties) {
    try {
        executor_data->getContext().getProperties().insert(properties.begin(), properties.end());
        executor_data->getExecutorId() = id;
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
