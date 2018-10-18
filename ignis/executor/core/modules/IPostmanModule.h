
#ifndef IGNIS_IPOSTMAN_H
#define IGNIS_IPOSTMAN_H

#include <thrift/transport/TServerSocket.h>
#include <thread>
#include "../../../rpc/executor/IPostmanModule.h"
#include "../../../IHeaders.h"
#include "../IMessage.h"
#include "IModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IPostmanModule : public IgnisModule, public ignis::rpc::executor::IPostmanModuleIf {
                public:
                    IPostmanModule(std::shared_ptr<IExecutorData> &executor_data);

                    virtual void start() override;

                    virtual void stop() override;

                    virtual void sendAll() override;

                    virtual void clearAll() override;

                    virtual ~IPostmanModule();

                private:
                    void threadServer();

                    void threadAccept(std::shared_ptr<transport::TTransport> trans);

                    int send(size_t id, IMessage &msg, int8_t compression);

                    std::shared_ptr<transport::TServerSocket> server;
                    std::shared_ptr<std::thread> thread_server;
                    bool started;
                };
            }
        }
    }
}

#endif
