
#ifndef IGNIS_ITYPESELECTOR_H
#define IGNIS_ITYPESELECTOR_H

#include "ISelectorUtils.h"
#include "ignis/executor/core/RTTInfo.h"
#include "ignis/executor/core/exception/ICompatibilityException.h"
#include "ignis/executor/core/modules/impl/ICacheImpl.h"
#include "ignis/executor/core/modules/impl/ICommImpl.h"
#include "ignis/executor/core/modules/impl/IIOImpl.h"
#include "ignis/executor/core/modules/impl/IMathImpl.h"
#include "ignis/executor/core/modules/impl/ISortImpl.h"
#include <functional>
#include <ignis/executor/core/modules/impl/IPipeImpl.h>
#include <ignis/executor/core/modules/impl/IReduceImpl.h>
#include <ignis/executor/core/modules/impl/IRepartitionImpl.h>
#include <memory>
#include <type_traits>
#include <vector>

namespace ignis {
    namespace executor {
        namespace core {
            namespace selector {
                class ITypeSelector {
                public:
                    virtual RTTInfo info() = 0;

                    virtual void cache(modules::impl::ICacheImpl &impl, const int64_t id, const int8_t level) = 0;

                    virtual void loadFromDisk(modules::impl::ICacheImpl &impl,
                                              const std::vector<std::string> &group) = 0;

                    virtual std::vector<std::string> getPartitions(modules::impl::ICommImpl &impl,
                                                                   const int8_t protocol, int64_t minPartitions) = 0;

                    virtual void setPartitions(modules::impl::ICommImpl &impl,
                                               const std::vector<std::string> &partitions) = 0;

                    virtual void driverGather(modules::impl::ICommImpl &impl, const std::string &id) = 0;

                    virtual void driverGather0(modules::impl::ICommImpl &impl, const std::string &id) = 0;

                    virtual void driverScatter(modules::impl::ICommImpl &impl, const std::string &id,
                                               int64_t partitions) = 0;

                    virtual void importData(modules::impl::ICommImpl &impl, const std::string &group, bool source,
                                            int64_t threads) = 0;

                    virtual void sort(modules::impl::ISortImpl &impl, bool ascending) = 0;

                    virtual void sort(modules::impl::ISortImpl &impl, bool ascending, int64_t numPartitions) = 0;

                    virtual void union_(modules::impl::IReduceImpl &impl, const std::string &other, bool preserveOrder) = 0;

                    virtual void join(modules::impl::IReduceImpl &impl, const std::string &other,
                                      int64_t numPartitions) = 0;

                    virtual void distinct(modules::impl::IReduceImpl &impl, int64_t numPartitions) = 0;

                    virtual void repartition(modules::impl::IRepartitionImpl &impl, int64_t numPartitions,
                                             bool preserveOrdering, bool global) = 0;

                    virtual void partitionByRandom(modules::impl::IRepartitionImpl &impl, int64_t numPartitions) = 0;

                    virtual void partitionByHash(modules::impl::IRepartitionImpl &impl, int64_t numPartitions) = 0;

                    virtual void take(modules::impl::IPipeImpl &impl, int64_t num) = 0;

                    virtual void top(modules::impl::ISortImpl &impl, int64_t num) = 0;

                    virtual void takeOrdered(modules::impl::ISortImpl &impl, int64_t num) = 0;

                    virtual void max(modules::impl::ISortImpl &impl) = 0;

                    virtual void min(modules::impl::ISortImpl &impl) = 0;

                    virtual void sample(modules::impl::IMathImpl &impl, bool withReplacement,
                                        const std::vector<int64_t> &num, int32_t seed) = 0;

                    virtual int64_t partitionApproxSize(modules::impl::IIOImpl &impl) = 0;

                    virtual void partitionObjectFile(modules::impl::IIOImpl &impl, const std::string &path,
                                                     int64_t first, int64_t partitions) = 0;

                    virtual void partitionJsonFile(modules::impl::IIOImpl &impl, const std::string &path, int64_t first,
                                                   int64_t partitions) = 0;

                    virtual void saveAsObjectFile(modules::impl::IIOImpl &impl, const std::string &path,
                                                  int8_t compression, int64_t first) = 0;

                    virtual void saveAsTextFile(modules::impl::IIOImpl &impl, const std::string &path,
                                                int64_t first) = 0;

                    virtual void saveAsJsonFile(modules::impl::IIOImpl &impl, const std::string &path, int64_t first,
                                                bool pretty) = 0;

                    /*Key-Value*/
                    virtual int64_t sampleByKeyFilter(modules::impl::IMathImpl &impl) = 0;

                    virtual void sampleByKey(modules::impl::IMathImpl &impl, bool withReplacement, int32_t seed) = 0;

                    virtual void countByKey(modules::impl::IMathImpl &impl) = 0;

                    virtual void countByValue(modules::impl::IMathImpl &impl) = 0;

                    virtual void keys(modules::impl::IPipeImpl &impl) = 0;

                    virtual void values(modules::impl::IPipeImpl &impl) = 0;

                    virtual void groupByKey(modules::impl::IReduceImpl &impl, int64_t numPartitions) = 0;

                    virtual void sortByKey(modules::impl::ISortImpl &impl, bool ascending) = 0;

                    virtual void sortByKey(modules::impl::ISortImpl &impl, bool ascending, int64_t numPartitions) = 0;
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

                    virtual std::vector<std::string> getPartitions(modules::impl::ICommImpl &impl,
                                                                   const int8_t protocol, int64_t minPartitions) {
                        return impl.getPartitions<Tp>(protocol, minPartitions);
                    }

                    virtual void setPartitions(modules::impl::ICommImpl &impl,
                                               const std::vector<std::string> &partitions) {
                        impl.setPartitions<Tp>(partitions);
                    }

                    virtual void driverGather(modules::impl::ICommImpl &impl, const std::string &id) {
                        impl.driverGather<Tp>(id);
                    }

                    virtual void driverGather0(modules::impl::ICommImpl &impl, const std::string &id) {
                        impl.driverGather0<Tp>(id);
                    }

                    virtual void driverScatter(modules::impl::ICommImpl &impl, const std::string &id,
                                               int64_t partitions) {
                        impl.driverScatter<Tp>(id, partitions);
                    }

                    virtual void importData(modules::impl::ICommImpl &impl, const std::string &group, bool source,
                                            int64_t threads) {
                        impl.importData<Tp>(group, source, threads);
                    }

                    virtual void sort(modules::impl::ISortImpl &impl, bool ascending) {
                        sort_check<Tp>(impl, nullptr, ascending);
                    }

                    virtual void sort(modules::impl::ISortImpl &impl, bool ascending, int64_t numPartitions) {
                        sort_check<Tp>(impl, nullptr, ascending, numPartitions);
                    }

                    virtual void union_(modules::impl::IReduceImpl &impl, const std::string &other, bool preserveOrder) {
                        impl.union_<Tp>(other, preserveOrder);
                    }

                    virtual void join(modules::impl::IReduceImpl &impl, const std::string &other,
                                      int64_t numPartitions) {
                        join_check<Tp>(impl, nullptr, other, numPartitions);
                    }

                    virtual void distinct(modules::impl::IReduceImpl &impl, int64_t numPartitions) {
                        distinct_check<Tp>(impl, nullptr, numPartitions);
                    }

                    virtual void repartition(modules::impl::IRepartitionImpl &impl, int64_t numPartitions,
                                             bool preserveOrdering, bool global) {
                        impl.repartition<Tp>(numPartitions, preserveOrdering, global);
                    }

                    virtual void partitionByRandom(modules::impl::IRepartitionImpl &impl, int64_t numPartitions) {
                        impl.partitionByRandom<Tp>(numPartitions);
                    }

                    virtual void partitionByHash(modules::impl::IRepartitionImpl &impl, int64_t numPartitions) {
                        partitionByHash_check<Tp>(impl, nullptr, numPartitions);
                    }

                    virtual void take(modules::impl::IPipeImpl &impl, int64_t num) { impl.take<Tp>(num); }

                    virtual void top(modules::impl::ISortImpl &impl, int64_t num) { top_check<Tp>(impl, nullptr, num); }

                    virtual void takeOrdered(modules::impl::ISortImpl &impl, int64_t num) {
                        takeOrdered_check<Tp>(impl, nullptr, num);
                    }

                    virtual void max(modules::impl::ISortImpl &impl) { max_check<Tp>(impl, nullptr); }

                    virtual void min(modules::impl::ISortImpl &impl) { min_check<Tp>(impl, nullptr); }

                    virtual void sample(modules::impl::IMathImpl &impl, bool withReplacement,
                                        const std::vector<int64_t> &num, int32_t seed) {
                        impl.sample<Tp>(withReplacement, num, seed);
                    }

                    virtual int64_t partitionApproxSize(modules::impl::IIOImpl &impl) {
                        return impl.partitionApproxSize<Tp>();
                    }

                    virtual void partitionObjectFile(modules::impl::IIOImpl &impl, const std::string &path,
                                                     int64_t first, int64_t partitions) {
                        impl.partitionObjectFile<Tp>(path, first, partitions);
                    }

                    virtual void partitionJsonFile(modules::impl::IIOImpl &impl, const std::string &path, int64_t first,
                                                   int64_t partitions) {
                        impl.partitionJsonFile<Tp>(path, first, partitions);
                    }

                    virtual void saveAsObjectFile(modules::impl::IIOImpl &impl, const std::string &path,
                                                  int8_t compression, int64_t first) {
                        impl.saveAsObjectFile<Tp>(path, compression, first);
                    }

                    virtual void saveAsTextFile(modules::impl::IIOImpl &impl, const std::string &path, int64_t first) {
                        impl.saveAsTextFile<Tp>(path, first);
                    }

                    virtual void saveAsJsonFile(modules::impl::IIOImpl &impl, const std::string &path, int64_t first,
                                                bool pretty) {
                        impl.saveAsJsonFile<Tp>(path, first, pretty);
                    }

                    /*Key-Value*/
                    virtual int64_t sampleByKeyFilter(modules::impl::IMathImpl &impl) {
                        return sampleByKeyFilter_check<Tp>(impl, nullptr);
                    }

                    virtual void sampleByKey(modules::impl::IMathImpl &impl, bool withReplacement, int32_t seed) {
                        sampleByKey_check<Tp>(impl, nullptr, withReplacement, seed);
                    }

                    virtual void countByKey(modules::impl::IMathImpl &impl) { countByKey_check<Tp>(impl, nullptr); }

                    virtual void countByValue(modules::impl::IMathImpl &impl) { countByValue_check<Tp>(impl, nullptr); }

                    virtual void keys(modules::impl::IPipeImpl &impl) { keys_check<Tp>(impl, nullptr); }

                    virtual void values(modules::impl::IPipeImpl &impl) { values_check<Tp>(impl, nullptr); }

                    virtual void groupByKey(modules::impl::IReduceImpl &impl, int64_t numPartitions) {
                        groupByKey_check<Tp>(impl, nullptr, numPartitions);
                    }

                    void sortByKey(modules::impl::ISortImpl &impl, bool ascending) {
                        sortByKey_check<Tp>(impl, nullptr, ascending);
                    }

                    void sortByKey(modules::impl::ISortImpl &impl, bool ascending, int64_t numPartitions) {
                        sortByKey_check<Tp>(impl, nullptr, ascending, numPartitions);
                    }

                private:
                    template<typename C>
                    void sort_check(modules::impl::ISortImpl &impl, typename IHasLess<C>::result val, bool ascending) {
                        impl.sort<Tp>(ascending);
                    }

                    template<typename C>
                    void sort_check(modules::impl::ISortImpl &impl, typename IHasLess<C>::result val, bool ascending,
                                    int64_t numPartitions) {
                        impl.sort<Tp>(ascending, numPartitions);
                    }

                    template<typename C>
                    void sort_check(...) {
                        throw exception::ICompatibilyException("sort", RTTInfo::from<C>());
                    }

                    template<typename C>
                    void join_check(modules::impl::IReduceImpl &impl, typename IHasHash<typename C::first_type>::result,
                                    const std::string &other, int64_t numPartitions) {
                        impl.join<Tp>(other, numPartitions);
                    }

                    template<typename C>
                    void join_check(...) {
                        throw exception::ICompatibilyException("join", RTTInfo::from<C>());
                    }

                    template<typename C>
                    void distinct_check(modules::impl::IReduceImpl &impl, typename IHasHash<C>::result,
                                        int64_t numPartitions) {
                        impl.distinct<Tp>(numPartitions);
                    }

                    template<typename C>
                    void distinct_check(...) {
                        throw exception::ICompatibilyException("distinct", RTTInfo::from<C>());
                    }

                    template<typename C>
                    void partitionByHash_check(modules::impl::IRepartitionImpl &impl,
                                                 typename IHasHash<C>::result val,
                                                 int64_t numPartitions) {
                        impl.partitionByHash<C>(numPartitions);
                    }

                    template<typename C>
                    void partitionByHash_check(...) {
                        throw exception::ICompatibilyException("partitionByHash", RTTInfo::from<C>());
                    }

                    template<typename C>
                    void top_check(modules::impl::ISortImpl &impl, typename IHasLess<C>::result val, int64_t num) {
                        impl.top<Tp>(num);
                    }

                    template<typename C>
                    void top_check(...) {
                        throw exception::ICompatibilyException("top", RTTInfo::from<C>());
                    }

                    template<typename C>
                    void takeOrdered_check(modules::impl::ISortImpl &impl, typename IHasLess<C>::result val,
                                           int64_t num) {
                        impl.takeOrdered<Tp>(num);
                    }

                    template<typename C>
                    void takeOrdered_check(...) {
                        throw exception::ICompatibilyException("takeOrdered", RTTInfo::from<C>());
                    }

                    template<typename C>
                    void max_check(modules::impl::ISortImpl &impl, typename IHasLess<C>::result val) {
                        impl.max<Tp>();
                    }

                    template<typename C>
                    void max_check(...) {
                        throw exception::ICompatibilyException("max", RTTInfo::from<C>());
                    }

                    template<typename C>
                    void min_check(modules::impl::ISortImpl &impl, typename IHasLess<C>::result val) {
                        impl.min<Tp>();
                    }

                    template<typename C>
                    void min_check(...) {
                        throw exception::ICompatibilyException("min", RTTInfo::from<C>());
                    }

                    /*Key-Value*/
                    template<typename C>
                    int64_t sampleByKeyFilter_check(modules::impl::IMathImpl &impl,
                                                    typename IHasHash<typename C::first_type>::result val) {
                        return impl.sampleByKeyFilter<C>();
                    }

                    template<typename C>
                    int64_t sampleByKeyFilter_check(...) {
                        throw exception::ICompatibilyException("sampleByKey", RTTInfo::from<C>());
                    }

                    template<typename C>
                    void sampleByKey_check(modules::impl::IMathImpl &impl,
                                           typename IHasHash<typename C::first_type>::result val, bool withReplacement,
                                           int32_t seed) {
                        impl.sampleByKey<C>(withReplacement, seed);
                    }

                    template<typename C>
                    void sampleByKey_check(...) {
                        throw exception::ICompatibilyException("sampleByKey", RTTInfo::from<C>());
                    }

                    template<typename C>
                    void countByKey_check(modules::impl::IMathImpl &impl,
                                          typename IHasHash<typename C::first_type>::result val) {
                        impl.countByKey<C>();
                    }

                    template<typename C>
                    void countByKey_check(...) {
                        throw exception::ICompatibilyException("countByKey", RTTInfo::from<C>());
                    }

                    template<typename C>
                    void countByValue_check(modules::impl::IMathImpl &impl,
                                            typename IHasHash<typename C::second_type>::result val) {
                        impl.countByValue<C>();
                    }

                    template<typename C>
                    void countByValue_check(...) {
                        throw exception::ICompatibilyException("countByValue", RTTInfo::from<C>());
                    }

                    template<typename C>
                    void keys_check(modules::impl::IPipeImpl &impl, typename C::second_type *val) {
                        impl.keys<C>();
                    }

                    template<typename C>
                    void keys_check(...) {
                        throw exception::ICompatibilyException("keys", RTTInfo::from<C>());
                    }

                    template<typename C>
                    void values_check(modules::impl::IPipeImpl &impl, typename C::second_type *val) {
                        impl.values<C>();
                    }

                    template<typename C>
                    void values_check(...) {
                        throw exception::ICompatibilyException("values", RTTInfo::from<C>());
                    }

                    template<typename C>
                    void groupByKey_check(modules::impl::IReduceImpl &impl,
                                          typename IHasHash<typename C::first_type>::result val,
                                          int64_t numPartitions) {
                        impl.groupByKey<C>(numPartitions);
                    }

                    template<typename C>
                    void groupByKey_check(...) {
                        throw exception::ICompatibilyException("groupByKey", RTTInfo::from<C>());
                    }

                    template<typename C>
                    void sortByKey_check(modules::impl::ISortImpl &impl,
                                         typename IHasLess<typename C::first_type>::result val, bool ascending) {
                        impl.sortByKey<Tp>(ascending);
                    }

                    template<typename C>
                    void sortByKey_check(modules::impl::ISortImpl &impl,
                                         typename IHasLess<typename C::first_type>::result val, bool ascending,
                                         int64_t numPartitions) {
                        impl.sortByKey<Tp>(ascending, numPartitions);
                    }

                    template<typename C>
                    void sortByKey_check(...) {
                        throw exception::ICompatibilyException("sortByKey", RTTInfo::from<C>());
                    }
                };

                template<typename Tp>
                std::shared_ptr<ITypeSelector> getType() {
                    return std::make_shared<ITypeSelectorImpl<Tp>>();
                }

                class ITypeSelectorExtractor {
                public:
                    template<typename Tp>
                    std::vector<std::shared_ptr<ITypeSelector>> extract() {
                        add<Tp>(nullptr);
                        return types;
                    }

                private:
                    std::vector<std::shared_ptr<ITypeSelector>> types;

                    template<typename Type>
                    void registerType() {
                        types.push_back(std::make_shared<ITypeSelectorImpl<Type>>());
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

                    template<typename IBeforeFunction>
                    void add(typename IBeforeFunction::_IBeforeFunction_type *f) {
                        registerType<typename IBeforeFunction::_T_type>();
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
            }// namespace selector
        }    // namespace core
    }        // namespace executor
}// namespace ignis

#endif
