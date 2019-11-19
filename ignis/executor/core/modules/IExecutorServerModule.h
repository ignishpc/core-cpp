
#ifndef IGNIS_IEXECUTORSERVERMODULE_H
#define IGNIS_IEXECUTORSERVERMODULE_H

#include "IModule.h"
#include "rpc/executor/IExecutorServerModule.h"
#include <thrift/TProcessor.h>
#include <thrift/server/TServer.h>

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IExecutorServerModule : public IModule, public rpc::executor::IExecutorServerModuleIf {
                public:

                    IExecutorServerModule(std::shared_ptr<IExecutorData> &executor_data);

                    void start(apache::thrift::TProcessor &procesor, int port);

                    void updateProperties(const std::map<std::string, std::string> &properties) override;

                    void stop() override;

                    bool test() override;

                    virtual ~IExecutorServerModule();

                private:
                    std::shared_ptr<apache::thrift::server::TServer> server;
                };
            }
        }
    }
}

#endif

