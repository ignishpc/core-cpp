
#ifndef IGNIS_ITYPESELECTOR_H
#define IGNIS_ITYPESELECTOR_H

#include <map>
#include <memory>
#include <functional>
#include <type_traits>
#include "ignis/executor/core/RTTInfo.h"
#include "ignis/executor/core/modules/impl/IIOImpl.h"
#include "ignis/executor/core/modules/impl/ICacheImpl.h"
#include "ignis/executor/core/modules/impl/ICommImpl.h"
#include "ignis/executor/core/modules/impl/ISortImpl.h"
#include "ignis/executor/core/exception/ICompatibilityException.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace selector {
                class ITypeSelector {
                public:
                    virtual RTTInfo info() = 0;

                    virtual void cache(modules::impl::ICacheImpl &impl, const int64_t id, const int8_t level) = 0;

                    virtual void
                    loadFromDisk(modules::impl::ICacheImpl &impl, const std::vector<std::string> &group) = 0;

                    virtual std::vector<std::string> getPartitions(modules::impl::ICommImpl &impl) = 0;

                    virtual void
                    setPartitions(modules::impl::ICommImpl &impl, const std::vector<std::string> &partitions) = 0;

                    virtual void driverGather(modules::impl::ICommImpl &impl, const std::string &id) = 0;

                    virtual void driverGather0(modules::impl::ICommImpl &impl, const std::string &id) = 0;

                    virtual void
                    driverScatter(modules::impl::ICommImpl &impl, const std::string &id, const int64_t dataId) = 0;

                    virtual void sort(modules::impl::ISortImpl &impl, bool ascending) = 0;

                    virtual void sort(modules::impl::ISortImpl &impl, bool ascending, int64_t numPartitions) = 0;

                    virtual int64_t partitionApproxSize(modules::impl::IIOImpl &impl) = 0;

                    virtual void partitionObjectFile(modules::impl::IIOImpl &impl, const std::string &path,
                                                     int64_t first, int64_t partitions) = 0;

                    virtual void partitionJsonFile(modules::impl::IIOImpl &impl, const std::string &path,
                                                   int64_t first, int64_t partitions) = 0;

                    virtual void saveAsObjectFile(modules::impl::IIOImpl &impl, const std::string &path,
                                                  int8_t compression, int64_t first) = 0;

                    virtual void
                    saveAsTextFile(modules::impl::IIOImpl &impl, const std::string &path, int64_t first) = 0;

                    virtual void
                    saveAsJsonFile(modules::impl::IIOImpl &impl, const std::string &path, int64_t first,
                                   bool pretty) = 0;

                };

                template<typename Tp>
                class ITypeSelectorImpl : public ITypeSelector {
                public:

                    virtual void checkAbstract() {
                        static_assert(!(std::is_abstract<Tp>::value), "abstract classes are not allowed");
                    }

                    virtual RTTInfo info() { return RTTInfo::from<Tp>(); };

                    virtual void cache(modules::impl::ICacheImpl &impl, const int64_t id, const int8_t level) {
                        impl.cache<Tp>(id, level);
                    }

                    virtual void loadFromDisk(modules::impl::ICacheImpl &impl, const std::vector<std::string> &group) {
                        impl.loadFromDisk<Tp>(group);
                    }

                    virtual std::vector<std::string> getPartitions(modules::impl::ICommImpl &impl) {
                        return impl.getPartitions<Tp>();
                    }

                    virtual void
                    setPartitions(modules::impl::ICommImpl &impl, const std::vector<std::string> &partitions) {
                        impl.setPartitions<Tp>(partitions);
                    }

                    virtual void driverGather(modules::impl::ICommImpl &impl, const std::string &id) {
                        impl.driverGather<Tp>(id);
                    }

                    virtual void driverGather0(modules::impl::ICommImpl &impl, const std::string &id) {
                        impl.driverGather0<Tp>(id);
                    }

                    virtual void
                    driverScatter(modules::impl::ICommImpl &impl, const std::string &id, const int64_t dataId) {
                        impl.driverScatter<Tp>(id, dataId);
                    }

                    virtual void sort(modules::impl::ISortImpl &impl, bool ascending) {
                        sort_check<Tp>(impl, nullptr, ascending);
                    }

                    virtual void sort(modules::impl::ISortImpl &impl, bool ascending, int64_t numPartitions) {
                        sort_check<Tp>(impl, nullptr, ascending, numPartitions);
                    }

                    virtual int64_t partitionApproxSize(modules::impl::IIOImpl &impl) {
                        return impl.partitionApproxSize<Tp>();
                    }

                    virtual void partitionObjectFile(modules::impl::IIOImpl &impl, const std::string &path,
                                                     int64_t first, int64_t partitions) {
                        impl.partitionObjectFile<Tp>(path, first, partitions);
                    }

                    virtual void partitionJsonFile(modules::impl::IIOImpl &impl, const std::string &path,
                                                   int64_t first, int64_t partitions) {
                        impl.partitionJsonFile<Tp>(path, first, partitions);
                    }

                    virtual void saveAsObjectFile(modules::impl::IIOImpl &impl, const std::string &path,
                                                  int8_t compression, int64_t first) {
                        impl.saveAsObjectFile<Tp>(path, compression, first);
                    }

                    virtual void saveAsTextFile(modules::impl::IIOImpl &impl, const std::string &path, int64_t first) {
                        impl.saveAsTextFile<Tp>(path, first);
                    }

                    virtual void
                    saveAsJsonFile(modules::impl::IIOImpl &impl, const std::string &path, int64_t first, bool pretty) {
                        impl.saveAsJsonFile<Tp>(path, first, pretty);
                    }

                private:

                    template<typename C>
                    void sort_check(modules::impl::ISortImpl &impl, decltype(std::declval<C>() < std::declval<C>()) *val, bool ascending) {
                        impl.sort<Tp>(ascending);
                    }

                    template<typename C>
                    void sort_check(modules::impl::ISortImpl &impl, decltype(std::declval<C>() < std::declval<C>()) *val, bool ascending,
                                    int64_t numPartitions) {
                        impl.sort<Tp>(ascending, numPartitions);
                    }

                    template<typename C>
                    void sort_check(...) { throw exception::ICompatibilyException("sort", RTTInfo::from<C>()); }

                };

                class ITypeSelectorExtractor {
                public:
                    typedef std::map<std::string, std::shared_ptr<ITypeSelector>> Args;

                    template<typename Tp>
                    Args extract() {
                        add<Tp>(nullptr);
                        return types;
                    }

                private:
                    Args types;

                    template<typename Type>
                    void registerType() {
                        auto type = std::make_shared<ITypeSelectorImpl<Type>>();
                        types[RTTInfo::from<Type>().getStandardName()] = std::static_pointer_cast<ITypeSelector>(type);
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

                    template<typename IVoidFunction0>
                    void add(typename IVoidFunction0::_IVoidFunction0_type *f) {
                        registerType<typename IVoidFunction0::_T_type>();
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

