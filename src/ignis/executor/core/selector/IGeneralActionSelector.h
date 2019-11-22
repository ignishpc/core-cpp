
#ifndef IGNIS_IGENERALACTIONSELECTOR_H
#define IGNIS_IGENERALACTIONSELECTOR_H

#include "ignis/executor/core/exception/ICompatibilityException.h"
#include "ignis/executor/core/RTTInfo.h"
#include "ignis/executor/core/modules/impl/IReduceImpl.h"
#include "ignis/executor/api/IReadIterator.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace selector {

                class IGeneralActionSelector {
                public:
                    virtual void reduce(modules::impl::IReduceImpl &impl) = 0;

                    virtual void treeReduce(modules::impl::IReduceImpl &impl, int64_t depth) = 0;
                };

                template<typename Tp>
                class IGeneralActionSelectorImpl : public IGeneralActionSelector {
                public:
                    virtual void reduce(modules::impl::IReduceImpl &impl) { reduce_check<Tp>(impl, nullptr); }

                    virtual void treeReduce(modules::impl::IReduceImpl &impl, int64_t depth) {
                        treeReduce_check<Tp>(impl, nullptr, depth);
                    }

                private:

                    template<typename Function>
                    void reduce_check(modules::impl::IReduceImpl &impl, typename Function::_IFunction2_type *val) {
                        reduce_check<Function>(impl, (typename Function::_T1_type *) nullptr,
                                               (typename Function::_T1_type *) nullptr);
                    }

                    template<typename Function>
                    void reduce_check(modules::impl::IReduceImpl &impl, typename Function::_T2_type *val2,
                                      typename Function::_R_type *val3) {
                        impl.reduce<Function>();
                    }

                    template<typename Function>
                    void reduce_check(...) {
                        throw exception::ICompatibilyException("reduce", RTTInfo::from<Function>());
                    }

                    template<typename Function>
                    void treeReduce_check(modules::impl::IReduceImpl &impl, typename Function::_IFunction2_type *val,
                                          int64_t depth) {
                        treeReduce_check<Function>(impl, (typename Function::_T1_type *) nullptr,
                                                   (typename Function::_T1_type *) nullptr, depth);
                    }

                    template<typename Function>
                    void treeReduce_check(modules::impl::IReduceImpl &impl, typename Function::_T2_type *val2,
                                          typename Function::_R_type *val3, int64_t depth) {
                        impl.treeReduce<Function>(depth);
                    }


                    template<typename Function>
                    void treeReduce_check(...) {
                        throw exception::ICompatibilyException("treeReduce", RTTInfo::from<Function>());
                    }

                };
            }
        }
    }
}

#endif
