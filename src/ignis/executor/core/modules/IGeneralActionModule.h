
#ifndef IGNIS_IGENERALACTIONMODULE_H
#define IGNIS_IGENERALACTIONMODULE_H

#include "IModule.h"
#include "ignis/rpc/executor/IGeneralActionModule.h"
#include "impl/IReduceImpl.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IGeneralActionModule : public modules::IModule, public rpc::executor::IGeneralActionModuleIf {
                public:

                    IGeneralActionModule(std::shared_ptr<IExecutorData> &executor_data);

                    void reduce(const rpc::ISource &function) override;

                    void treeReduce(const rpc::ISource &function) override;

                    void collect() override;

                    void aggregate(const rpc::ISource &zero, const rpc::ISource &seqOp, const rpc::ISource &combOp) override;

                    void treeAggregate(const rpc::ISource &zero, const rpc::ISource &seqOp, const rpc::ISource &combOp) override;

                    void fold(const rpc::ISource &zero, const rpc::ISource &function) override;

                    void treeFold(const rpc::ISource &zero, const rpc::ISource &function) override;

                    void take(int64_t num) override;

                    void foreach_(const rpc::ISource &function) override;

                    void foreachPartition(const rpc::ISource &function) override;

                    void top(int64_t num) override;

                    void top2(int64_t num, const rpc::ISource &comp) override;

                    void takeOrdered(int64_t num) override;

                    void takeOrdered2(int64_t num, const rpc::ISource &comp) override;

                    void keys() override;

                    void values() override;

                    virtual ~IGeneralActionModule();

                private:
                    impl::IReduceImpl reduce_impl;
                    impl::IPipeImpl pipe_impl;
                    impl::ISortImpl sort_impl;
                };
            }
        }
    }
}


#endif
