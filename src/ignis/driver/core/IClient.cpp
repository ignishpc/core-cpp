
#include "IClient.h"
#include <chrono>
#include <thread>
#include <thrift/protocol/TMultiplexedProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TZlibTransport.h>
#include <thrift/protocol/TCompactProtocol.h>

using namespace ignis::driver::core;
using namespace ignis::rpc::driver;
using namespace apache::thrift;


class DisableThriftLog {

public:
    static void nothing(const char *msg) {}

    DisableThriftLog() { apache::thrift::GlobalOutput.setOutputFunction(&DisableThriftLog::nothing); }

    virtual ~DisableThriftLog() {
        apache::thrift::GlobalOutput.setOutputFunction(&apache::thrift::TOutput::errorTimeWrapper);
    }
};

IClient::IClient(int port, int compression)
    : transport(std::make_shared<transport::TZlibTransport>(std::make_shared<transport::TSocket>("localhost", port),
                                                            128, 1024, 128, 1024, compression)),
      protocol(std::make_shared<protocol::TCompactProtocol>(transport)),
      backendService(std::make_shared<IBackendServiceClient>(
              std::make_shared<protocol::TMultiplexedProtocol>(protocol, "IBackend"))),
      propertiesService(std::make_shared<IPropertiesServiceClient>(
              std::make_shared<protocol::TMultiplexedProtocol>(protocol, "IProperties"))),
      clusterService(std::make_shared<IClusterServiceClient>(
              std::make_shared<protocol::TMultiplexedProtocol>(protocol, "ICluster"))),
      workerService(std::make_shared<IWorkerServiceClient>(
              std::make_shared<protocol::TMultiplexedProtocol>(protocol, "IWorker"))),
      dataframeService(std::make_shared<IDataFrameServiceClient>(
              std::make_shared<protocol::TMultiplexedProtocol>(protocol, "IDataFrame"))) {

    DisableThriftLog disable;
    for (int i = 0; i < 10; i++) {
        try {
            transport->open();
            break;
        } catch (transport::TTransportException &ex) {
            if (i == 9) { throw ex; }
            std::this_thread::sleep_for(std::chrono::seconds(i));
        }
    }
}

IBackendServiceClient &IClient::getBackendService() { return *backendService; }

IPropertiesServiceClient &IClient::getPropertiesService() { return *propertiesService; }

IClusterServiceClient &IClient::getClusterService() { return *clusterService; }

IWorkerServiceClient &IClient::getWorkerService() { return *workerService; }

IDataFrameServiceClient &IClient::getDataFrameService() { return *dataframeService; }

IClient::~IClient() { transport->close(); }