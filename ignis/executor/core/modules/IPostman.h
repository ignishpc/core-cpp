
#ifndef IGNIS_IPOSTMAN_H
#define IGNIS_IPOSTMAN_H

#include <vector>
#include <thrift/transport/TServerSocket.h>
#include "IModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IPostman : public IgnisModule {
                public:
                    IPostman(std::shared_ptr<IExecutorData> &executor_data);

                    void start();

                    void stop();

                    void sendAll();

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
