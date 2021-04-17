
#ifndef IGNIS_EXECUTORDATA_H
#define IGNIS_EXECUTORDATA_H

#include "ILibraryLoader.h"
#include "ILog.h"
#include "IMpi.h"
#include "IPartitionTools.h"
#include "IPropertyParser.h"
#include "exception/IInvalidArgument.h"
#include "ignis/executor/api/IContext.h"
#include "ignis/rpc/ISource_types.h"
#include "storage/IPartition.h"
#include "storage/IVoidPartition.h"
#include <map>
#include <utility>

namespace ignis {
    namespace executor {
        namespace core {
            namespace selector {
                class ISelectorGroup;

                class ITypeSelector;
            }// namespace selector

            class IExecutorData {
            public:
                IExecutorData();

                template<typename Tp>
                std::shared_ptr<storage::IPartitionGroup<Tp>> getPartitions(bool no_check = false);

                template<typename Tp>
                std::shared_ptr<storage::IPartitionGroup<Tp>> getAndDeletePartitions();

                template<typename Tp>
                void setPartitions(const std::shared_ptr<storage::IPartitionGroup<Tp>> &group);

                bool hasPartitions();

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

                std::vector<std::string> loadLibraryFunctions(const std::string &path);

                std::shared_ptr<selector::ISelectorGroup> loadLibrary(const rpc::ISource &source,
                                                                      bool withBackup = true, bool fast = false);

                void loadParameters(const rpc::ISource &source);

                void reloadLibraries();

                void registerType(const std::shared_ptr<selector::ITypeSelector> &type);

                std::shared_ptr<selector::ITypeSelector> getType(const std::string &id);

                api::IContext &getContext();

                IPropertyParser &getProperties();

                IPartitionTools &getPartitionTools();

                IMpi &mpi();

                void setCores(int cores);

                int getCores();

                int getMpiCores();

                void enableMpiCores();

                void setMpiGroup(const MPI::Intracomm &group);

                virtual ~IExecutorData();

            private:
                std::shared_ptr<void> partitions;
                std::map<std::string, std::shared_ptr<IVariable>> variables;
                std::map<std::string,
                         std::pair<std::shared_ptr<selector::ITypeSelector>, std::shared_ptr<selector::ISelectorGroup>>>
                        types;
                std::map<std::string, std::shared_ptr<selector::ISelectorGroup>> functions;
                ILibraryLoader library_loader;
                IPropertyParser properties;
                IPartitionTools partition_tools;
                IMpi _mpi;
                api::IContext context;
                int cores;
            };
        }// namespace core
    }    // namespace executor
}// namespace ignis

#include "IExecutorData.tcc"

#endif
