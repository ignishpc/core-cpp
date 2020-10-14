
#ifndef IGNIS_IGENERALMODULE_H
#define IGNIS_IGENERALMODULE_H

#include "IModule.h"
#include "ignis/rpc/executor/IGeneralModule.h"
#include "impl/IPipeImpl.h"
#include "impl/IReduceImpl.h"
#include "impl/ISortImpl.h"

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

                    void keyBy(const ::ignis::rpc::ISource &src) override;

                    void mapPartitions(const rpc::ISource &function, bool preservesPartitioning) override;

                    void mapPartitionsWithIndex(const rpc::ISource &function, bool preservesPartitioning) override;

                    void mapExecutor(const rpc::ISource &function) override;

                    void mapExecutorTo(const rpc::ISource &function) override;

                    void groupBy(const rpc::ISource &function, int64_t numPartitions) override;

                    void sort(bool ascending) override;

                    void sort2(bool ascending, int64_t numPartitions) override;

                    void sortBy(const rpc::ISource &function, bool ascending) override;

                    void sortBy3(const rpc::ISource &function, bool ascending, int64_t numPartitions) override;

                    void flatMapValues(const ::ignis::rpc::ISource &src) override;

                    void mapValues(const ::ignis::rpc::ISource &src) override;

                    void groupByKey(const int64_t numPartitions) override;

                    void reduceByKey(const ::ignis::rpc::ISource &src, const int64_t numPartitions,
                                     bool localReduce) override;

                    void aggregateByKey(const ::ignis::rpc::ISource &zero, const ::ignis::rpc::ISource &seqOp,
                                        const int64_t numPartitions) override;

                    void aggregateByKey4(const ::ignis::rpc::ISource &zero, const ::ignis::rpc::ISource &seqOp,
                                         const ::ignis::rpc::ISource &combOp, const int64_t numPartitions) override;

                    void foldByKey(const ::ignis::rpc::ISource &zero, const ::ignis::rpc::ISource &src,
                                   const int64_t numPartitions, bool localFold) override;

                    void sortByKey(const bool ascending) override;

                    void sortByKey2a(const bool ascending, const int64_t numPartitions) override;

                    void sortByKey2b(const ::ignis::rpc::ISource &src, const bool ascending) override;

                    void sortByKey3(const ::ignis::rpc::ISource &src, const bool ascending,
                                    const int64_t numPartitions) override;

                    virtual ~IGeneralModule();

                private:
                    impl::IPipeImpl pipe_impl;
                    impl::ISortImpl sort_impl;
                    impl::IReduceImpl reduce_impl;
                };
            }// namespace modules
        }    // namespace core
    }        // namespace executor
}// namespace ignis

#endif
