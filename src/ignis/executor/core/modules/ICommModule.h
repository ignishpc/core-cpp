
#ifndef IGNIS_ICOMMMODULE_H
#define IGNIS_ICOMMMODULE_H

#include "IModule.h"
#include "ignis/rpc/executor/ICommModule.h"
#include "impl/ICommImpl.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class ICommModule : public modules::IModule, public rpc::executor::ICommModuleIf {
                public:
                    ICommModule(std::shared_ptr<IExecutorData> &executor_data);

                    void openGroup(std::string &_return) override;

                    void closeGroup() override;

                    void joinToGroup(const std::string &id, const bool leader) override;

                    void joinToGroupName(const std::string &id, const bool leader, const std::string &name) override;

                    bool hasGroup(const std::string &name) override;

                    void destroyGroup(const std::string &name) override;

                    void destroyGroups() override;

                    int8_t getProtocol() override;

                    void getPartitions(std::vector<std::string> &_return, const int8_t protocol) override;

                    void getPartitions2(std::vector<std::string> &_return, const int8_t protocol,
                                        int64_t minPartitions) override;

                    void setPartitions(const std::vector<std::string> &partitions) override;

                    void setPartitions2(const std::vector<std::string> &partitions, const rpc::ISource &src) override;

                    void driverGather(const std::string &group, const rpc::ISource &src) override;

                    void driverGather0(const std::string &group, const rpc::ISource &src) override;

                    void driverScatter(const std::string &group, int64_t partitions) override;

                    void driverScatter3(const std::string &group, int64_t partitions, const rpc::ISource &src) override;

                    virtual void importData(const std::string& group, const bool source, const int64_t threads) override;

                    virtual void importData4(const std::string& group, const bool source, const int64_t threads, const  rpc::ISource& src) override;

                    virtual ~ICommModule();

                private:
                    impl::ICommImpl impl;
                };
            }// namespace modules
        }    // namespace core
    }        // namespace executor
}// namespace ignis

#endif
