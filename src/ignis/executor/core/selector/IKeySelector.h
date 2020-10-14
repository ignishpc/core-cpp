
#ifndef IGNIS_IKEYSELECTOR_H
#define IGNIS_IKEYSELECTOR_H

#include "ignis/executor/core/RTTInfo.h"
#include "ignis/executor/core/exception/ICompatibilityException.h"
#include "ignis/executor/core/modules/impl/IPipeImpl.h"
#include <utility>


namespace ignis {
    namespace executor {
        namespace core {
            namespace selector {

                class IKeySelector {
                public:
                    virtual void flatMapValues(modules::impl::IPipeImpl &impl) { error(); };

                    virtual void mapValues(modules::impl::IPipeImpl &impl) { error(); };

                    virtual void reduceByKey(modules::impl::IReduceImpl &impl, int64_t numPartitions,
                                             bool localReduce) {
                        error();
                    };

                    virtual void aggregateByKey(modules::impl::IReduceImpl &impl, int64_t numPartitions, bool hashing) {
                        error();
                    };

                    virtual void foldByKey(modules::impl::IReduceImpl &impl, int64_t numPartitions, bool localFold) {
                        error();
                    };


                private:
                    inline void error() {
                        throw exception::ILogicError("function not registered with ignis_export_with_key");
                    }
                };

                template<typename K, typename Tp>
                class IKeySelectorImpl : public IKeySelector {
                public:
                    virtual void flatMapValues(modules::impl::IPipeImpl &impl) {
                        flatMapValues_check<Tp>(impl, nullptr, nullptr);
                    }

                    virtual void mapValues(modules::impl::IPipeImpl &impl) { mapValues_check<Tp>(impl, nullptr); }

                    virtual void reduceByKey(modules::impl::IReduceImpl &impl, int64_t numPartitions,
                                             bool localReduce) {
                        reduceByKey_check<Tp>(impl, nullptr, numPartitions, localReduce);
                    };

                    virtual void aggregateByKey(modules::impl::IReduceImpl &impl, int64_t numPartitions, bool hashing) {
                        aggregateByKey_check<Tp>(impl, nullptr, numPartitions, hashing);
                    };

                    virtual void foldByKey(modules::impl::IReduceImpl &impl, int64_t numPartitions, bool localFold) {
                        foldByKey_check<Tp>(impl, nullptr, numPartitions, localFold);
                    };

                private:
                    template<typename Function>
                    void mapValues_check(modules::impl::IPipeImpl &impl, typename Function::_IFunction_type *val) {
                        impl.mapValues<std::pair<K, typename Function::_T_type>, Function>();
                    }

                    template<typename Function>
                    void mapValues_check(...) {
                        throw exception::ICompatibilyException("mapValues", RTTInfo::from<Function>());
                    }

                    template<typename Function>
                    void flatMapValues_check(modules::impl::IPipeImpl &impl,
                                             typename Function::_IFunction_type::_R_type::iterator *val,
                                             typename Function::_IFunction_type::_R_type::value_type *val2) {
                        impl.registerType(
                                getType<std::pair<K, typename Function::_IFunction_type::_R_type::value_type>>());
                        impl.flatMapValues<std::pair<K, typename Function::_T_type>, Function>();
                    }

                    template<typename Function>
                    void flatMapValues_check(...) {
                        throw exception::ICompatibilyException("flatMapValues", RTTInfo::from<Function>());
                    }

                    template<typename Function>
                    void reduceByKey_check(modules::impl::IReduceImpl &impl, typename Function::_IFunction2_type *val,
                                           int64_t numPartitions, bool localReduce) {
                        reduceByKey_check<Function>(impl, (typename Function::_T1_type *) nullptr,
                                                    (typename Function::_T1_type *) nullptr, nullptr, numPartitions,
                                                    localReduce);
                    }

                    template<typename Function>
                    void reduceByKey_check(modules::impl::IReduceImpl &impl, typename Function::_T2_type *val2,
                                           typename Function::_R_type *val3,
                                           decltype(&std::hash<typename Function::_T1_type>::operator()) *val4,
                                           int64_t numPartitions, bool localReduce) {
                        impl.reduceByKey<std::pair<K, typename Function::_T1_type>, Function>(numPartitions,
                                                                                              localReduce);
                    }

                    template<typename Function>
                    void reduceByKey_check(...) {
                        throw exception::ICompatibilyException("reduceByKey", RTTInfo::from<Function>());
                    }

                    template<typename Function>
                    void aggregateByKey_check(modules::impl::IReduceImpl &impl,
                                              typename Function::_IFunction2_type *val, int64_t numPartitions,
                                              bool hashing) {
                        aggregateByKey_check<Function>(impl, (typename Function::_T2_type *) nullptr,
                                                       (typename Function::_T1_type *) nullptr, nullptr, numPartitions,
                                                       hashing);
                    }

                    template<typename Function>
                    void aggregateByKey_check(modules::impl::IReduceImpl &impl, typename Function::_T2_type *val1,
                                              typename Function::_R_type *val2,
                                              decltype(&std::hash<typename Function::_T1_type>::operator()) *val3,
                                              int64_t numPartitions, bool hashing) {
                        impl.aggregateByKey<std::pair<K, typename Function::_T2_type>, Function>(numPartitions,
                                                                                                 hashing);
                    }

                    template<typename Function>
                    void aggregateByKey_check(...) {
                        throw exception::ICompatibilyException("aggregateByKey", RTTInfo::from<Function>());
                    }

                    template<typename Function>
                    void foldByKey_check(modules::impl::IReduceImpl &impl, typename Function::_IFunction2_type *val,
                                         int64_t numPartitions, bool localFold) {
                        foldByKey_check<Function>(impl, (typename Function::_T1_type *) nullptr,
                                                  (typename Function::_T1_type *) nullptr, nullptr, numPartitions,
                                                  localFold);
                    }

                    template<typename Function>
                    void foldByKey_check(modules::impl::IReduceImpl &impl, typename Function::_T2_type *val2,
                                         typename Function::_R_type *val3,
                                         decltype(&std::hash<typename Function::_T1_type>::operator()) *val4,
                                         int64_t numPartitions, bool localFold) {
                        impl.foldByKey<std::pair<K, typename Function::_T1_type>, Function>(numPartitions, localFold);
                    }

                    template<typename Function>
                    void foldByKey_check(...) {
                        throw exception::ICompatibilyException("foldByKey", RTTInfo::from<Function>());
                    }
                };

            }// namespace selector
        }    // namespace core
    }        // namespace executor
}// namespace ignis
#endif
