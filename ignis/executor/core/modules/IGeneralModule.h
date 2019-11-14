
#ifndef IGNIS_IGENERALMODULE_H
#define IGNIS_IGENERALMODULE_H

#include "IModule.h"
#include "impl/IGroupByImpl.h"
#include "impl/ISortImpl.h"
#include "impl/IPipeImpl.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IGeneralModule : public modules::IModule {
                public:

                    IGeneralModule(std::shared_ptr<IExecutorData> &executor_data);

                    void map_(const rpc::ISource &function);

                    void filter(const rpc::ISource &function);

                    void flatmap(const rpc::ISource &function);

                    void mapPartitions(const rpc::ISource &function, bool preservesPartitioning);

                    void mapPartitionsWithIndex(const rpc::ISource &function, bool preservesPartitioning);

                    void applyPartition(const rpc::ISource &function);

                    void groupBy(const rpc::ISource &function);

                    void groupBy(const rpc::ISource &function, int64_t numPartitions);

                    void sort(bool ascending);

                    void sort(bool ascending, int64_t numPartitions);

                    void sortBy(const rpc::ISource &function, bool ascending);

                    void sortBy(const rpc::ISource &function, bool ascending, int64_t numPartitions);

                    virtual ~IGeneralModule();

                private:
                    impl::IGroupByImpl group_by_impl;
                    impl::IPipeImpl pipe_impl;
                    impl::ISortImpl sort_impl;
                };
            }
        }
    }
}

#endif
