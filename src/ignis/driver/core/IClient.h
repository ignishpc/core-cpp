
#ifndef IGNIS_ICLIENT_H
#define IGNIS_ICLIENT_H

#include <memory>
#include <thrift/transport/TTransport.h>
#include <thrift/protocol/TCompactProtocol.h>
#include "ignis/rpc/driver/IBackendService.h"
#include "ignis/rpc/driver/IPropertiesService.h"
#include "ignis/rpc/driver/IClusterService.h"
#include "ignis/rpc/driver/IWorkerService.h"
#include "ignis/rpc/driver/IDataFrameService.h"

namespace ignis {
    namespace driver {
        namespace core {
            class IClient {
            public:

                IClient(int port, int compression);

                rpc::driver::IBackendServiceClient &getBackendService();

                rpc::driver::IPropertiesServiceClient &getPropertiesService();

                rpc::driver::IClusterServiceClient &getClusterService();

                rpc::driver::IWorkerServiceClient &getWorkerService();

                rpc::driver::IDataFrameServiceClient &getDataFrameService();

                virtual ~IClient();

            private:
                std::shared_ptr<apache::thrift::transport::TTransport> transport;
                std::shared_ptr<apache::thrift::protocol::TProtocol> protocol;
                std::shared_ptr<rpc::driver::IBackendServiceClient> backendService;
                std::shared_ptr<rpc::driver::IPropertiesServiceClient> propertiesService;
                std::shared_ptr<rpc::driver::IClusterServiceClient> clusterService;
                std::shared_ptr<rpc::driver::IWorkerServiceClient> workerService;
                std::shared_ptr<rpc::driver::IDataFrameServiceClient> dataframeService;
            };
        }
    }
}

#endif
