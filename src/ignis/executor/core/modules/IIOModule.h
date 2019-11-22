
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

                    void textFile(const std::string &path, const int64_t partitions);

                    void openPartitionObjectFile(const std::string &path, int64_t first, int64_t partitions) override;

                    void openPartitionObjectFileFunction(const rpc::ISource &function, const std::string &path,
                                                         int64_t first, int64_t partitions) override;

                    void saveAsPartitionObjectFile(const std::string &path, int8_t compression, int64_t first) override;

                    void saveAsTextFile(const std::string &path, int64_t first) override;

                    void saveAsJsonFile(const std::string &path, int64_t first) override;

                    virtual ~IIOModule();

                private:
                    impl::IIOImpl impl;
                };
            }
        }
    }
}
#endif
