
#ifndef IGNIS_IGENERALMODULE_H
#define IGNIS_IGENERALMODULE_H

#include "IModule.h"
#include "ignis/rpc/executor/IGeneralModule.h"
#include "impl/IGroupByImpl.h"
#include "impl/ISortImpl.h"
#include "impl/IPipeImpl.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IGeneralModule : public modules::IModule, public rpc::executor::IGeneralModuleIf {
                public:

                    IGeneralModule(std::shared_ptr<IExecutorData> &executor_data);

                    void map_(const rpc::ISource &function) override;

                    void filter(const rpc::ISource &function) override;

                    void flatmap(const rpc::ISource &function) override;

                    void mapPartitions(const rpc::ISource &function, bool preservesPartitioning) override;

                    void mapPartitionsWithIndex(const rpc::ISource &function, bool preservesPartitioning) override;

                    void applyPartition(const rpc::ISource &function) override;

                    void groupBy(const rpc::ISource &function) override;

                    void groupBy2(const rpc::ISource &function, int64_t numPartitions) override;

                    void sort(bool ascending) override;

                    void sort2(bool ascending, int64_t numPartitions) override;

                    void sortBy(const rpc::ISource &function, bool ascending) override;

                    void sortBy3(const rpc::ISource &function, bool ascending, int64_t numPartitions) override;

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
