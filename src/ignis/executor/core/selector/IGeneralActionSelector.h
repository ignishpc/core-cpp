
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

                    virtual void treeReduce(modules::impl::IReduceImpl &impl) = 0;

                    virtual void zero(modules::impl::IReduceImpl &impl) = 0;

                    virtual void aggregate(modules::impl::IReduceImpl &impl) = 0;

                    virtual void fold(modules::impl::IReduceImpl &impl) = 0;

                    virtual void treeFold(modules::impl::IReduceImpl &impl) = 0;

                    virtual void foreach(modules::impl::IPipeImpl &impl) = 0;

                    virtual void foreachPartition(modules::impl::IPipeImpl &impl) = 0;

                    virtual void top(modules::impl::ISortImpl &impl, int64_t n) = 0;

                    virtual void takeOrdered(modules::impl::ISortImpl &impl, int64_t n) = 0;

                    virtual void max(modules::impl::ISortImpl &impl) = 0;

                    virtual void min(modules::impl::ISortImpl &impl) = 0;
                };

                template<typename Tp>
                class IGeneralActionSelectorImpl : public IGeneralActionSelector {
                public:
                    virtual void reduce(modules::impl::IReduceImpl &impl) { reduce_check<Tp>(impl, nullptr); }

                    virtual void treeReduce(modules::impl::IReduceImpl &impl) { treeReduce_check<Tp>(impl, nullptr); }

                    virtual void zero(modules::impl::IReduceImpl &impl) { zero_check<Tp>(impl, nullptr); }

                    virtual void aggregate(modules::impl::IReduceImpl &impl) { aggregate_check<Tp>(impl, nullptr); }

                    virtual void fold(modules::impl::IReduceImpl &impl) { fold_check<Tp>(impl, nullptr); }

                    virtual void treeFold(modules::impl::IReduceImpl &impl) { treeFold_check<Tp>(impl, nullptr); }

                    virtual void foreach(modules::impl::IPipeImpl &impl) { foreach_check<Tp>(impl, nullptr); }

                    virtual void foreachPartition(modules::impl::IPipeImpl &impl) {
                        foreachPartition_check<Tp>(impl, nullptr);
                    }

                    virtual void top(modules::impl::ISortImpl &impl, int64_t n) { top_check<Tp>(impl, nullptr, n); }

                    virtual void takeOrdered(modules::impl::ISortImpl &impl, int64_t n) {
                        takeOrdered_check<Tp>(impl, nullptr, n);
                    }

                    virtual void max(modules::impl::ISortImpl &impl) { max_check<Tp>(impl, nullptr); }

                    virtual void min(modules::impl::ISortImpl &impl) { min_check<Tp>(impl, nullptr); }

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
                    void treeReduce_check(modules::impl::IReduceImpl &impl, typename Function::_IFunction2_type *val) {
                        treeReduce_check<Function>(impl, (typename Function::_T1_type *) nullptr,
                                                   (typename Function::_T1_type *) nullptr);
                    }

                    template<typename Function>
                    void treeReduce_check(modules::impl::IReduceImpl &impl, typename Function::_T2_type *val2,
                                          typename Function::_R_type *val3) {
                        impl.treeReduce<Function>();
                    }


                    template<typename Function>
                    void treeReduce_check(...) {
                        throw exception::ICompatibilyException("treeReduce", RTTInfo::from<Function>());
                    }

                    template<typename Function>
                    void zero_check(modules::impl::IReduceImpl &impl, typename Function::_IFunction0_type *val) {
                        impl.zero<Function>();
                    }

                    template<typename Function>
                    void zero_check(...) {
                        throw exception::ICompatibilyException("zero", RTTInfo::from<Function>());
                    }

                    template<typename Function>
                    void aggregate_check(modules::impl::IReduceImpl &impl, typename Function::_IFunction2_type *val) {
                        aggregate_check<Function>(impl, (typename Function::_T2_type *) nullptr,
                                                  (typename Function::_T1_type *) nullptr);
                    }

                    template<typename Function>
                    void aggregate_check(modules::impl::IReduceImpl &impl, typename Function::_T2_type *val1,
                                         typename Function::_R_type *val2) {
                        impl.aggregate<Function>();
                    }

                    template<typename Function>
                    void aggregate_check(...) {
                        throw exception::ICompatibilyException("aggregate", RTTInfo::from<Function>());
                    }

                    template<typename Function>
                    void fold_check(modules::impl::IReduceImpl &impl, typename Function::_IFunction2_type *val) {
                        fold_check<Function>(impl, (typename Function::_T1_type *) nullptr,
                                             (typename Function::_T1_type *) nullptr);
                    }

                    template<typename Function>
                    void fold_check(modules::impl::IReduceImpl &impl, typename Function::_T2_type *val2,
                                    typename Function::_R_type *val3) {
                        impl.fold<Function>();
                    }

                    template<typename Function>
                    void fold_check(...) {
                        throw exception::ICompatibilyException("fold", RTTInfo::from<Function>());
                    }

                    template<typename Function>
                    void treeFold_check(modules::impl::IReduceImpl &impl, typename Function::_IFunction2_type *val) {
                        treeFold_check<Function>(impl, (typename Function::_T1_type *) nullptr,
                                                 (typename Function::_T1_type *) nullptr);
                    }

                    template<typename Function>
                    void treeFold_check(modules::impl::IReduceImpl &impl, typename Function::_T2_type *val2,
                                        typename Function::_R_type *val3) {
                        impl.treeFold<Function>();
                    }

                    template<typename Function>
                    void treeFold_check(...) {
                        throw exception::ICompatibilyException("treeFold", RTTInfo::from<Function>());
                    }

                    template<typename Function>
                    void foreach_check(modules::impl::IPipeImpl &impl, typename Function::_IVoidFunction_type *val) {
                        impl.foreach<Function>();
                    }

                    template<typename Function>
                    void foreach_check(...) {
                        throw exception::ICompatibilyException("foreach", RTTInfo::from<Function>());
                    }

                    template<typename Function>
                    void foreachPartition_check(modules::impl::IPipeImpl &impl,
                                                typename Function::_IVoidFunction_type::_T_type::_IReadIterator_type *val) {
                        impl.foreachPartition<Function>();
                    }

                    template<typename Function>
                    void foreachPartition_check(...) {
                        throw exception::ICompatibilyException("foreachPartition", RTTInfo::from<Function>());
                    }

                    template<typename Function>
                    void
                    top_check(modules::impl::ISortImpl &impl, typename Function::_IFunction2_type *val, int64_t n) {
                        top_check<Function>(impl, (typename Function::_T2_type *) nullptr,
                                            (typename Function::_R_type *) nullptr, n);
                    }

                    template<typename Function>
                    void top_check(modules::impl::ISortImpl &impl, typename Function::_T1_type *val, bool *val2,
                                   int64_t n) {
                        impl.topBy<Function>(n);
                    }

                    template<typename Function>
                    void top_check(...) {
                        throw exception::ICompatibilyException("top", RTTInfo::from<Function>());
                    }

                    template<typename Function>
                    void
                    takeOrdered_check(modules::impl::ISortImpl &impl, typename Function::_IFunction2_type *val, int64_t n) {
                        takeOrdered_check<Function>(impl, (typename Function::_T2_type *) nullptr,
                                            (typename Function::_R_type *) nullptr, n);
                    }

                    template<typename Function>
                    void takeOrdered_check(modules::impl::ISortImpl &impl, typename Function::_T1_type *val, bool *val2,
                                   int64_t n) {
                        impl.takeOrderedBy<Function>(n);
                    }

                    template<typename Function>
                    void takeOrdered_check(...) {
                        throw exception::ICompatibilyException("takeOrdered", RTTInfo::from<Function>());
                    }

                    template<typename Function>
                    void
                    max_check(modules::impl::IReduceImpl &impl, typename Function::_IFunction2_type *val, int64_t n) {
                        max_check<Function>(impl, val, (typename Function::_T2_type *) nullptr,
                                            (typename Function::_R_type *) nullptr, n);
                    }

                    template<typename Function>
                    void max_check(modules::impl::ISortImpl &impl, typename Function::_T1_type *val, bool *val2,
                                   int64_t n) {
                        impl.maxBy<Function>(n);
                    }

                    template<typename Function>
                    void max_check(...) {
                        throw exception::ICompatibilyException("max", RTTInfo::from<Function>());
                    }

                    template<typename Function>
                    void
                    min_check(modules::impl::IReduceImpl &impl, typename Function::_IFunction2_type *val, int64_t n) {
                        min_check<Function>(impl, val, (typename Function::_T2_type *) nullptr,
                                            (typename Function::_R_type *) nullptr, n);
                    }

                    template<typename Function>
                    void min_check(modules::impl::ISortImpl &impl, typename Function::_T1_type *val, bool *val2,
                                   int64_t n) {
                        impl.minBy<Function>(n);
                    }

                    template<typename Function>
                    void min_check(...) {
                        throw exception::ICompatibilyException("min", RTTInfo::from<Function>());
                    }

                };
            }
        }
    }
}

#endif
