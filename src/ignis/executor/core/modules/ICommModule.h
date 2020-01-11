
#ifndef IGNIS_ICOMMMODULE_H
#define IGNIS_ICOMMMODULE_H

#include "IModule.h"
#include "ignis/rpc/executor/ICommModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class ICommModule : public modules::IModule, public rpc::executor::ICommModuleIf {
                public:

                    ICommModule(std::shared_ptr<IExecutorData> &executor_data);

                    void createGroup(std::string &_return) override;

                    void joinGroupMembers(const std::string &group, const int64_t size) override;

                    void joinToGroup(const std::string &group, const std::string &id) override;

                    bool hasGroup(const std::string &id) override;

                    void destroyGroup(const std::string &id) override;

                    void destroyGroups() override;

                    void getPartitions(std::vector<std::string> &_return) override;

                    void setPartitions(const std::vector<std::string> &partitions) override;

                    void driverGather(const std::string &id) override;

                    void driverGather0(const std::string &id) override;

                    void driverScatter(const std::string &id) override;

                    virtual ~ICommModule();

                };
            }
        }
    }
}

#endif

