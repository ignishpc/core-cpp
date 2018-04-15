
#ifndef IGNIS_ISERVERMODULE_H
#define IGNIS_ISERVERMODULE_H

#include <thrift/TProcessor.h>
#include <thrift/server/TServer.h>
#include "IModule.h"
#include "../../../rpc/executor/IServerModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IServerModule : public IgnisModule, public ignis::rpc::executor::IServerModuleIf{
                public:
                    IServerModule(std::shared_ptr<IExecutorData> &executor_data);

                    void start(std::shared_ptr<apache::thrift::TProcessor> processor, int port);

                    void stop();

                    bool test();

                    void setProperties(const std::map<std::string, std::string> & properties);

                private:
                    apache::thrift::server::TServer* server_ptr;
                };
            }
        }
    }
}


#endif
