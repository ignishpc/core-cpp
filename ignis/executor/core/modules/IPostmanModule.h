
#ifndef IGNIS_IPOSTMAN_H
#define IGNIS_IPOSTMAN_H

#include <vector>
#include <thrift/transport/TServerSocket.h>
#include "IModule.h"
#include "../../../rpc/executor/IPostmanModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IPostmanModule : public IgnisModule, public ignis::rpc::executor::IPostmanModuleIf {
                public:
                    IPostmanModule(std::shared_ptr<IExecutorData> &executor_data);

                    void start();

                    void stop();

                    void sendAll();

                private:
                    void threadServer();

                    void threadAccept(std::shared_ptr<apache::thrift::transport::TTransport> trans);

                    std::shared_ptr<apache::thrift::transport::TServerSocket> server;
                    bool started;
                };
            }
        }
    }
}

#endif
