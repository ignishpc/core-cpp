
#ifndef IGNIS_IVALUESELECTOR_H
#define IGNIS_IVALUESELECTOR_H

#include "ignis/executor/core/RTTInfo.h"
#include "ignis/executor/core/exception/ICompatibilityException.h"
#include "ignis/executor/core/modules/impl/IPipeImpl.h"
#include <utility>


namespace ignis {
    namespace executor {
        namespace core {
            namespace selector {

                class IValueSelector {
                public:
                    virtual void sortByKey(modules::impl::ISortImpl &impl, bool ascending) { error(); };

                    virtual void sortByKey(modules::impl::ISortImpl &impl, bool ascending, int64_t numPartitions) {
                        error();
                    };

                private:
                    inline void error() {
                        throw exception::ILogicError("function not registered with ignis_export_with_value");
                    }
                };

                template<typename V, typename Tp>
                class IValueSelectorImpl : public IValueSelector {
                public:
                    virtual void sortByKey(modules::impl::ISortImpl &impl, bool ascending) {
                        sortByKey_check<Tp>(impl, nullptr, ascending);
                    };

                    virtual void sortByKey(modules::impl::ISortImpl &impl, bool ascending, int64_t numPartitions) {
                        sortByKey_check<Tp>(impl, nullptr, ascending, numPartitions);
                    };

                private:
                    template<typename Function>
                    void sortByKey_check(modules::impl::ISortImpl &impl, typename Function::_IFunction2_type *val,
                                         bool ascending) {
                        sortByKey_check<Function>(impl, (typename Function::_T2_type *) nullptr,
                                                  (typename Function::_R_type *) nullptr, ascending);
                    }

                    template<typename Function>
                    void sortByKey_check(modules::impl::ISortImpl &impl, typename Function::_T1_type **val, bool *val2,
                                         bool ascending) {
                        impl.sortByKeyBy<std::pair<typename Function::_T1_type, V>, Function>(ascending);
                    }

                    template<typename Function>
                    void sortByKey_check(modules::impl::ISortImpl &impl, typename Function::_IFunction2_type *val,
                                         bool ascending, int64_t numPartitions) {
                        sortByKey_check<Function>(impl, (typename Function::_T2_type *) nullptr,
                                                  (typename Function::_R_type *) nullptr, ascending, numPartitions);
                    }

                    template<typename Function>
                    void sortByKey_check(modules::impl::ISortImpl &impl, typename Function::_T1_type *val, bool *val2,
                                         bool ascending, int64_t numPartitions) {
                        impl.sortByKeyBy<std::pair<typename Function::_T1_type, V>, Function>(ascending, numPartitions);
                    }

                    template<typename Function>
                    void sortByKey_check_check(...) {
                        throw exception::ICompatibilyException("sortByKey", RTTInfo::from<Function>());
                    }
                };

            }// namespace selector
        }    // namespace core
    }        // namespace executor
}// namespace ignis
#endif
