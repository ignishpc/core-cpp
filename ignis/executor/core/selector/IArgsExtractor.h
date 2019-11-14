
#ifndef IGNIS_IARGSEXTRACTOR_H
#define IGNIS_IARGSEXTRACTOR_H

#include <map>
#include <memory>
#include <functional>
#include "executor/core/RTTInfo.h"
#include "executor/core/modules/impl/ISortImpl.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace selector {
                class IArgsType {
                public:
                    virtual RTTInfo info() = 0;

                    virtual void sort(modules::impl::ISortImpl &impl, bool ascending) = 0;

                };

                template<typename Tp>
                class IArgsTypeImpl : public IArgsType {
                public:
                    virtual RTTInfo info() { return RTTInfo::from<Tp>(); };

                    virtual void sort(modules::impl::ISortImpl &impl, bool ascending) {
                        sort_check<Tp>(impl, nullptr, ascending);
                    }

                private:

                    template<typename C>
                    void sort_check(modules::impl::ISortImpl &impl, std::less<C> *val, bool ascending) {
                        impl.sort<Tp>(ascending);
                    }

                    template<typename C>
                    void sort_check(...) { /**/ }

                };

                class IArgsExtractor {
                public:
                    typedef std::map<std::string, std::shared_ptr<IArgsType>> Args;

                    template<typename Tp>
                    Args extract() {
                        add<Tp>(nullptr);
                        return types;
                    }

                private:
                    Args types;

                    template<typename Type>
                    void registerType() {
                        auto type = std::make_shared<IArgsTypeImpl<Type>>();
                        types[RTTInfo::from<Type>().getStandardName()] = std::static_pointer_cast<IArgsType>(type);
                    }

                    template<typename IFlatMapFunction>
                    void add(typename IFlatMapFunction::_IFlatMapFunction_type *f) {
                        registerType<typename IFlatMapFunction::_T_type>();
                        registerType<typename IFlatMapFunction::_R_type>();
                    }

                    template<typename IFunction>
                    void add(typename IFunction::_IFunction_type *f) {
                        registerType<typename IFunction::_T_type>();
                        registerType<typename IFunction::_R_type>();
                    }

                    template<typename IFunction0>
                    void add(typename IFunction0::_IFunction0_type *f) {
                        registerType<typename IFunction0::_R_type>();
                    }

                    template<typename IFunction2>
                    void add(typename IFunction2::_IFunction2_type *f) {
                        registerType<typename IFunction2::_T1_type>();
                        registerType<typename IFunction2::_T2_type>();
                        registerType<typename IFunction2::_R_type>();
                    }

                    template<typename IVoidFunction>
                    void add(typename IVoidFunction::_IVoidFunction_type *f) {
                        registerType<typename IVoidFunction::_T_type>();
                    }

                    template<typename IVoidFunction2>
                    void add(typename IVoidFunction2::_IVoidFunction2_type *f) {
                        registerType<typename IVoidFunction2::_T1_type>();
                        registerType<typename IVoidFunction2::_T2_type>();
                    }

                    template<typename None>
                    void add(...) {}
                };
            }
        }
    }
}

#endif

