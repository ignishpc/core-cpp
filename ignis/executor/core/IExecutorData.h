
#ifndef IGNIS_EXECUTORDATA_H
#define IGNIS_EXECUTORDATA_H

#include "storage/IPartition.h"
#include "ILibraryLoader.h"
#include "IPropertyParser.h"
#include "executor/api/IContext.h"
#include "executor/core/exception/IInvalidArgument.h"
#include "rpc/ISource_types.h"
#include "ILog.h"
#include "IMpi.h"
#include <map>

namespace ignis {
    namespace executor {
        namespace core {
            namespace selector {
                class ISelectorGroup;

                class ITypeSelector;
            }

            class IExecutorData {
            public:
                IExecutorData();

                template<typename Tp>
                std::shared_ptr<storage::IPartitionGroup<Tp>> getPartitions(bool no_check = false) {
                    auto group = std::static_pointer_cast<storage::IPartitionGroup<Tp>>(partitions);
                    if (!no_check && group->elemType() != RTTInfo::from<Tp>()) {
                        throw exception::IInvalidArgument(
                                "Error: " + group->elemType().getStandardName() + " cannot be cast to " +
                                RTTInfo::from<Tp>().getStandardName());
                    }
                    return group;
                }

                template<typename Tp>
                std::shared_ptr<storage::IPartitionGroup<Tp>>
                setPartitions(std::shared_ptr<storage::IPartitionGroup<Tp>> &group) {
                    auto old = partitions;
                    partitions = std::static_pointer_cast<void>(group);
                    return std::static_pointer_cast<storage::IPartitionGroup<Tp>>(old);
                }

                void deletePartitions();

                template<typename Tp>
                void setVariable(const std::string key, const std::shared_ptr<Tp> &value) {
                    variables[key] = std::static_pointer_cast<std::shared_ptr<Tp>>(value);
                }

                template<typename Tp>
                std::shared_ptr<Tp> &getVariable(const std::string key) {
                    return std::static_pointer_cast<std::shared_ptr<Tp>>(variables[key]);
                }

                template<typename Tp>
                std::shared_ptr<Tp> removeVariable(const std::string key) {
                    auto value = variables[key];
                    variables.erase(key);
                    return std::static_pointer_cast<std::shared_ptr<Tp>>(value);
                }

                int64_t clearVariables();

                int64_t nextPartititonId();

                std::shared_ptr<selector::ISelectorGroup> loadLibrary(const rpc::ISource &source);

                std::shared_ptr<selector::ITypeSelector> getType(const std::string &id);

                api::IContext &getContext();

                IPropertyParser &getProperties();

                IMpi mpi();

                void setCores(int cores);

                virtual ~IExecutorData();

            private:
                std::shared_ptr<void> partitions;
                std::map<std::string, std::shared_ptr<void>> variables;
                std::map<std::string, std::pair<
                        std::shared_ptr<selector::ITypeSelector>,
                        std::shared_ptr<selector::ISelectorGroup>>
                > types;
                ILibraryLoader library_loader;
                IPropertyParser properties;
                IMpi _mpi;
                api::IContext context;
                int partition_id_gen;
            };
        }
    }
}

#endif
