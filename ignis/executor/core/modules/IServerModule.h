
#ifndef EXECUTORCPP_ISERVERMODULE_H
#define EXECUTORCPP_ISERVERMODULE_H

#include <thrift/TProcessor.h>
#include <thrift/server/TServer.h>
#include "IModule.h"


namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IServerModule : public IgnisModule{
                public:
                    IServerModule(std::shared_ptr<IExecutorData> &executor_data, int port);

                    void start(std::shared_ptr<apache::thrift::TProcessor> &processor);

                    void stop();

                private:
                    int port;
                    apache::thrift::server::TServer* server_ptr;
                };
            }
        }
    }
}


#endif
