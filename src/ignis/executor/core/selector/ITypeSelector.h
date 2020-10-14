
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
#include <map>
#include <memory>
#include <type_traits>

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

                    virtual void send(modules::impl::ICommImpl &impl, const std::string &id, int64_t partition,
                                      int64_t dest, int64_t tag) = 0;

                    virtual void recv(modules::impl::ICommImpl &impl, const std::string &id, int64_t partition,
                                      int64_t source, int64_t tag) = 0;

                    virtual void sort(modules::impl::ISortImpl &impl, bool ascending) = 0;

                    virtual void sort(modules::impl::ISortImpl &impl, bool ascending, int64_t numPartitions) = 0;

                    virtual void take(modules::impl::IPipeImpl &impl, int64_t num) = 0;

                    virtual void top(modules::impl::ISortImpl &impl, int64_t num) = 0;

                    virtual void takeOrdered(modules::impl::ISortImpl &impl, int64_t num) = 0;

                    virtual int64_t count(modules::impl::IMathImpl &impl) = 0;

                    virtual void max(modules::impl::ISortImpl &impl) = 0;

                    virtual void min(modules::impl::ISortImpl &impl) = 0;

                    virtual void sample(modules::impl::IMathImpl &impl, bool withReplacement, double fraction,
                                        int32_t seed) = 0;

                    virtual void takeSample(modules::impl::IMathImpl &impl, bool withReplacement, int64_t num,
                                            int32_t seed) = 0;

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

                    virtual void send(modules::impl::ICommImpl &impl, const std::string &id, int64_t partition,
                                      int64_t dest, int64_t tag) {
                        impl.send<Tp>(id, partition, dest, tag);
                    }

                    virtual void recv(modules::impl::ICommImpl &impl, const std::string &id, int64_t partition,
                                      int64_t source, int64_t tag) {
                        impl.recv<Tp>(id, partition, source, tag);
                    }

                    virtual void sort(modules::impl::ISortImpl &impl, bool ascending) {
                        sort_check<Tp>(impl, nullptr, ascending);
                    }

                    virtual void sort(modules::impl::ISortImpl &impl, bool ascending, int64_t numPartitions) {
                        sort_check<Tp>(impl, nullptr, ascending, numPartitions);
                    }

                    virtual void take(modules::impl::IPipeImpl &impl, int64_t num) { impl.take<Tp>(num); }

                    virtual void top(modules::impl::ISortImpl &impl, int64_t num) { top_check<Tp>(impl, nullptr, num); }

                    virtual void takeOrdered(modules::impl::ISortImpl &impl, int64_t num) {
                        takeOrdered_check<Tp>(impl, nullptr, num);
                    }

                    virtual int64_t count(modules::impl::IMathImpl &impl) { return impl.count<Tp>(); }

                    virtual void max(modules::impl::ISortImpl &impl) { max_check<Tp>(impl, nullptr); }

                    virtual void min(modules::impl::ISortImpl &impl) { min_check<Tp>(impl, nullptr); }

                    virtual void sample(modules::impl::IMathImpl &impl, bool withReplacement, double fraction,
                                        int32_t seed) {
                        impl.sample<Tp>(withReplacement, fraction, seed);
                    }

                    virtual void takeSample(modules::impl::IMathImpl &impl, bool withReplacement, int64_t num,
                                            int32_t seed) {
                        impl.takeSample<Tp>(withReplacement, num, seed);
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
                    void sampleByKey_check(modules::impl::IMathImpl &impl, typename C::second_type *val,
                                           bool withReplacement, int32_t seed) {
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
            }// namespace selector
        }    // namespace core
    }        // namespace executor
}// namespace ignis

#endif
