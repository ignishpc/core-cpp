
#ifndef IGNIS_EXECUTORDATA_H
#define IGNIS_EXECUTORDATA_H

#include "storage/IPartition.h"
#include "storage/IVoidPartition.h"
#include "ILibraryLoader.h"
#include "IPropertyParser.h"
#include "IPartitionTools.h"
#include "ignis/executor/api/IContext.h"
#include "ignis/executor/core/exception/IInvalidArgument.h"
#include "ignis/rpc/ISource_types.h"
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
                std::shared_ptr<storage::IPartitionGroup<Tp>> getPartitions(bool no_check = false);

                template<typename Tp>
                std::shared_ptr<storage::IPartitionGroup<Tp>>
                setPartitions(const std::shared_ptr<storage::IPartitionGroup<Tp>> &group);

                void deletePartitions();

                template<typename Tp>
                void setVariable(const std::string key, const Tp &value);

                template<typename Tp>
                void setVariable(const std::string key, Tp &&value);

                template<typename Tp>
                Tp &getVariable(const std::string key);

                bool hasVariable(const std::string key);

                void removeVariable(const std::string key);

                int64_t clearVariables();

                std::string infoDirectory();

                std::shared_ptr<selector::ISelectorGroup> loadLibrary(const rpc::ISource &source);

                void reloadLibraries();

                void registerType(const std::shared_ptr<selector::ITypeSelector>&type);

                std::shared_ptr<selector::ITypeSelector> getType(const std::string &id);

                api::IContext &getContext();

                IPropertyParser &getProperties();

                IPartitionTools &getPartitionTools();

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
                IPartitionTools partition_tools;
                IMpi _mpi;
                api::IContext context;
            };
        }
    }
}

#include "IExecutorData.tcc"

#endif
