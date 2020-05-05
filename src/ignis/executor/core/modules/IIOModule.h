
#ifndef IGNIS_IIOMODULE_H
#define IGNIS_IIOMODULE_H

#include "IModule.h"
#include "ignis/rpc/executor/IIOModule.h"
#include "impl/IIOImpl.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IIOModule : public IModule, public rpc::executor::IIOModuleIf {
                public:

                    IIOModule(std::shared_ptr<IExecutorData> &executor_data);

                    void loadClass(const  ::ignis::rpc::ISource& src);

                    int64_t partitionCount() override;

                    int64_t partitionApproxSize() override;

                    void textFile(const std::string& path) override;

                    void textFile2(const std::string& path, const int64_t minPartitions) override;

                    void partitionObjectFile(const std::string& path, const int64_t first, const int64_t partitions) override;

                    void partitionObjectFile4(const std::string& path, const int64_t first, const int64_t partitions, const  rpc::ISource& src) override;

                    void partitionTextFile(const std::string& path, const int64_t first, const int64_t partitions) override;

                    void partitionJsonFile4a(const std::string& path, const int64_t first, const int64_t partitions, const bool objectMapping) override;

                    void partitionJsonFile4b(const std::string& path, const int64_t first, const int64_t partitions, const  rpc::ISource& src) override;

                    void saveAsObjectFile(const std::string& path, const int8_t compression, const int64_t first) override;

                    void saveAsTextFile(const std::string& path, const int64_t first) override;

                    void saveAsJsonFile(const std::string& path, const int64_t first, const bool pretty) override;

                    virtual ~IIOModule();

                private:
                    impl::IIOImpl impl;
                };
            }
        }
    }
}
#endif
