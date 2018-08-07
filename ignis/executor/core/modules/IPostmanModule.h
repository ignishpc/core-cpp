
#ifndef IGNIS_IPOSTMAN_H
#define IGNIS_IPOSTMAN_H

#include <vector>
#include <thrift/transport/TServerSocket.h>
#include "IModule.h"
#include "../../../rpc/executor/IPostmanModule.h"
#include "../storage/IMessage.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IPostmanModule : public IgnisModule, public ignis::rpc::executor::IPostmanModuleIf {
                public:
                    IPostmanModule(std::shared_ptr<IExecutorData> &executor_data);

                    virtual void start();

                    virtual void stop() override;

                    virtual void sendAll() override;

                    virtual void clearAll() override;

                    virtual ~IPostmanModule();

                private:
                    void threadServer();

                    void threadAccept(size_t id, std::shared_ptr<apache::thrift::transport::TTransport> trans);

                    int send(size_t id, storage::IMessage &msg, int8_t compression);

                    std::shared_ptr<apache::thrift::transport::TServerSocket> server;
                    bool started;
                };
            }
        }
    }
}

#endif
