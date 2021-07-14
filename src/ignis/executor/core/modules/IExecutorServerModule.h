
#ifndef IGNIS_IEXECUTORSERVERMODULE_H
#define IGNIS_IEXECUTORSERVERMODULE_H

#include "IModule.h"
#include "ignis/rpc/executor/IExecutorServerModule.h"
#include <thrift/processor/TMultiplexedProcessor.h>
#include <thrift/server/TServer.h>

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IExecutorServerModule : public IModule, public rpc::executor::IExecutorServerModuleIf {
                public:
                    IExecutorServerModule(std::shared_ptr<IExecutorData> &executor_data);

                    void serve(const std::string &name, int port, int compression);

                    void start(const std::map<std::string, std::string> &properties,
                               const std::map<std::string, std::string> &env) override;

                    void stop() override;

                    bool test() override;

                    virtual ~IExecutorServerModule();

                protected:
                    virtual void createServices(apache::thrift::TMultiplexedProcessor &processor);

                private:
                    std::shared_ptr<apache::thrift::server::TServer> server;
                    std::shared_ptr<apache::thrift::TMultiplexedProcessor> processor;
                };
            }// namespace modules
        }    // namespace core
    }        // namespace executor
}// namespace ignis

#endif
