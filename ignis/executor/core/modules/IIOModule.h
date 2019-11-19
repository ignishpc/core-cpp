
#ifndef IGNIS_IIOMODULE_H
#define IGNIS_IIOMODULE_H

#include "IModule.h"
#include "impl/IIOImpl.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IIOModule : public IModule {

                    IIOModule(std::shared_ptr<IExecutorData> &executor_data);

                    void openPartitionObjectFile(const std::string &path, int64_t first, int64_t partitions);

                    void openPartitionObjectFileFunction(const rpc::ISource &function, const std::string &path,
                                                         int64_t first, int64_t partitions);

                    void saveAsPartitionObjectFile(const std::string &path, int8_t compression, int64_t first);

                    void saveAsTextFile(const std::string &path, int64_t first);

                    void saveAsJsonFile(const std::string &path, int64_t first);

                    virtual ~IIOModule();

                private:
                    impl::IIOImpl impl;
                };
            }
        }
    }
}
#endif
