
#ifndef IGNIS_IGENERALACTIONMODULE_H
#define IGNIS_IGENERALACTIONMODULE_H

#include "IModule.h"
#include "impl/IReduceImpl.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IGeneralActionModule : public modules::IModule {
                public:

                    IGeneralActionModule(std::shared_ptr<IExecutorData> &executor_data);

                    void reduce(const rpc::ISource &function);

                    void treeReduce(const rpc::ISource &function, int64_t depth);

                    void collect();

                    void aggregate(const rpc::ISource &seqOp, const rpc::ISource &combOp);

                    void treeAggregate(const rpc::ISource &seqOp, const rpc::ISource &combOp, int64_t depth);

                    void fold(const rpc::ISource &function);

                    void take(int64_t num);

                    void foreach(const rpc::ISource &function);

                    void foreachPartition(const rpc::ISource &function);

                    void top(int64_t num);

                    void top(int64_t num, const rpc::ISource &comp);

                    virtual ~IGeneralActionModule();
                private:
                    impl::IReduceImpl reduce_impl;

                };
            }
        }
    }
}


#endif
