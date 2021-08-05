
#ifndef IGNIS_IMATHMODULE_H
#define IGNIS_IMATHMODULE_H

#include "IModule.h"
#include "ignis/rpc/executor/IMathModule.h"
#include "impl/IMathImpl.h"
#include "impl/ISortImpl.h"
#include "impl/IReduceImpl.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IMathModule : public IModule, public rpc::executor::IMathModuleIf {
                public:
                    IMathModule(std::shared_ptr<IExecutorData> &executorData);

                    void sample(const bool withReplacement, const std::vector<int64_t> & num, const int32_t seed) override;

                    int64_t count() override;

                    void max() override;

                    void min() override;

                    void max1(const rpc::ISource &cmp) override;

                    void min1(const rpc::ISource &cmp) override;

                    void sampleByKey(const bool withReplacement, const rpc::ISource &fractions,
                                     const int32_t seed) override;

                    void countByKey() override;

                    void countByValue() override;

                    virtual ~IMathModule();

                private:
                    impl::IMathImpl math_impl;
                    impl::ISortImpl sort_impl;
                    impl::IReduceImpl reduce_impl;
                };
            }// namespace modules
        }    // namespace core
    }        // namespace executor
}// namespace ignis

#endif
