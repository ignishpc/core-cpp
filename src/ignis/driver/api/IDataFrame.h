
#ifndef IGNIS_IDATAFRAME_H
#define IGNIS_IDATAFRAME_H

#include "ISource.h"
#include "Ignis.h"
#include "ignis/rpc/ISource_types.h"
#include "ignis/rpc/driver/IDataFrameService_types.h"
#include <unordered_map>
#include <vector>

namespace ignis {
    namespace driver {
        namespace api {

            enum ICacheLevel { NO_CACHE = 0, PRESERVE = 1, MEMORY = 2, RAW_MEMORY = 3, DISK = 4 };

            class IAbstractDataFrame {
            protected:
                rpc::driver::IDataFrameId id;

                rpc::driver::IDataFrameId getId();

                IAbstractDataFrame(const rpc::driver::IDataFrameId &id);

                core::IDriverContext &driverContext();

                void setName(const std::string &name);

                void persist(const ICacheLevel &level);

                void cache();

                void unpersist();

                void uncache();

                /*IO*/
                rpc::driver::IDataFrameId repartitionAbs(int64_t numPartitions);

                rpc::driver::IDataFrameId coalesceAbs(int64_t numPartitions, bool shuffle);

                int64_t partitions();

                void saveAsObjectFile(const std::string &path, int compression = 6);

                void saveAsTextFile(const std::string &path);

                void saveAsJsonFile(const std::string &path, bool pretty = true);

                /*General*/
                rpc::driver::IDataFrameId mapAbs(const ISource &src);

                rpc::driver::IDataFrameId filterAbs(const ISource &src);

                rpc::driver::IDataFrameId flatmapAbs(const ISource &src);

                rpc::driver::IDataFrameId keyByAbs(const ISource &src);

                rpc::driver::IDataFrameId mapPartitionsAbs(const ISource &src, bool preservesPartitioning);

                rpc::driver::IDataFrameId mapPartitionsWithIndexAbs(const ISource &src, bool preservesPartitioning);

                rpc::driver::IDataFrameId mapExecutorAbs(const ISource &src);

                rpc::driver::IDataFrameId mapExecutorAbsTo(const ISource &src);

                rpc::driver::IDataFrameId groupByAbs(const ISource &src);

                rpc::driver::IDataFrameId groupByAbs(const ISource &src, int64_t numPartitions);

                rpc::driver::IDataFrameId sortAbs(bool ascending);

                rpc::driver::IDataFrameId sortAbs(bool ascending, int64_t numPartitions);

                rpc::driver::IDataFrameId sortByAbs(const ISource &src, bool ascending);

                rpc::driver::IDataFrameId sortByAbs(const ISource &src, bool ascending, int64_t numPartitions);

                rpc::driver::IDataFrameId unionAbs(const rpc::driver::IDataFrameId &other, bool preserveOrder);

                rpc::driver::IDataFrameId unionAbs(const rpc::driver::IDataFrameId &other, bool preserveOrder,
                                                   int64_t numPartitions);

                rpc::driver::IDataFrameId unionAbs(const rpc::driver::IDataFrameId &other, bool preserveOrder,
                                                   const ISource &src);

                rpc::driver::IDataFrameId unionAbs(const rpc::driver::IDataFrameId &other, bool preserveOrder,
                                                   int64_t numPartitions, const ISource &src);

                rpc::driver::IDataFrameId distinctAbs();

                rpc::driver::IDataFrameId distinctAbs(int64_t numPartitions);

                rpc::driver::IDataFrameId distinctAbs(const ISource &src);

                rpc::driver::IDataFrameId distinctAbs(int64_t numPartitions, const ISource &src);

                /*General Action*/
                int64_t reduceAbs(const ISource &src, const rpc::ISource &tp);

                int64_t treeReduceAbs(const ISource &src, const rpc::ISource &tp);

                int64_t collectAbs(const rpc::ISource &tp);

                int64_t aggregateAbs(const ISource &zero, const ISource &seqOp, const ISource &combOp,
                                     const rpc::ISource &tp);

                int64_t treeAggregateAbs(const ISource &zero, const ISource &seqOp, const ISource &combOp,
                                         const rpc::ISource &tp);

                int64_t foldAbs(const ISource &zero, const ISource &src, const rpc::ISource &tp);

                int64_t treeFoldAbs(const ISource &zero, const ISource &src, const rpc::ISource &tp);

                int64_t takeAbs(int64_t num, const rpc::ISource &tp);

                void foreach (const ISource &src);

                void foreachPartition(const ISource &src);

                void foreachExecutor(const ISource &src);

                int64_t topAbs(int64_t num, const rpc::ISource &tp);

                int64_t topAbs(int64_t num, const ISource &cmp, const rpc::ISource &tp);

                int64_t takeOrderedAbs(int64_t num, const rpc::ISource &tp);

                int64_t takeOrderedAbs(int64_t num, const ISource &cmp, const rpc::ISource &tp);

                /*Math*/
                rpc::driver::IDataFrameId sampleAbs(bool withReplacement, double fraction, int seed);

                int64_t takeSampleAbs(bool withReplacement, int64_t num, int seed, const rpc::ISource &tp);

                int64_t count();

                int64_t maxAbs(const rpc::ISource &tp);

                int64_t maxAbs(const ISource &cmp, const rpc::ISource &tp);

                int64_t minAbs(const rpc::ISource &tp);

                int64_t minAbs(const ISource &cmp, const rpc::ISource &tp);

                /*Key-Value*/
                rpc::driver::IDataFrameId joinAbs(const rpc::driver::IDataFrameId &other);

                rpc::driver::IDataFrameId joinAbs(const rpc::driver::IDataFrameId &other, int64_t numPartitions);

                rpc::driver::IDataFrameId joinAbs(const rpc::driver::IDataFrameId &other, const ISource &src);

                rpc::driver::IDataFrameId joinAbs(const rpc::driver::IDataFrameId &other, int64_t numPartitions,
                                                  const ISource &src);

                rpc::driver::IDataFrameId flatMapValuesAbs(const ISource &src);

                rpc::driver::IDataFrameId mapValuesAbs(const ISource &src);

                rpc::driver::IDataFrameId groupByKeyAbs();

                rpc::driver::IDataFrameId groupByKeyAbs(int64_t numPartitions);

                rpc::driver::IDataFrameId groupByKeyAbs(const ISource &src);

                rpc::driver::IDataFrameId groupByKeyAbs(int64_t numPartitions, const ISource &src);

                rpc::driver::IDataFrameId reduceByKeyAbs(const ISource &src, bool localReduce);

                rpc::driver::IDataFrameId reduceByKeyAbs(const ISource &src, int64_t numPartitions, bool localReduce);

                rpc::driver::IDataFrameId aggregateByKeyAbs(const ISource &zero, const ISource &seqOp);

                rpc::driver::IDataFrameId aggregateByKeyAbs(const ISource &zero, const ISource &seqOp,
                                                            int64_t numPartitions);

                rpc::driver::IDataFrameId aggregateByKeyAbs(const ISource &zero, const ISource &seqOp,
                                                            const ISource &combOp);

                rpc::driver::IDataFrameId aggregateByKeyAbs(const ISource &zero, const ISource &seqOp,
                                                            const ISource &combOp, int64_t numPartitions);

                rpc::driver::IDataFrameId foldByKeyAbs(const ISource &zero, const ISource &src, bool localFold);

                rpc::driver::IDataFrameId foldByKeyAbs(const ISource &zero, const ISource &src, int64_t numPartitions,
                                                       bool localFold);

                rpc::driver::IDataFrameId sortByKeyAbs(bool ascending);

                rpc::driver::IDataFrameId sortByKeyAbs(bool ascending, int64_t numPartitions);

                rpc::driver::IDataFrameId sortByKeyAbs(const ISource &src, bool ascending);

                rpc::driver::IDataFrameId sortByKeyAbs(const ISource &src, bool ascending, int64_t numPartitions);

                int64_t keysAbs(const rpc::ISource &tp);

                int64_t valuesAbs(const rpc::ISource &tp);

                rpc::driver::IDataFrameId sampleByKeyAbs(bool withReplacement, const ISource &fractions, int seed);

                int64_t countByKeyAbs(const rpc::ISource &tp);

                int64_t countByValueAbs(const rpc::ISource &tp);
            };

            class IWorker;

            template<typename Key, typename Value>
            class IPairDataFrame;

            template<typename Tp>
            class _toPair {};

            template<typename Key, typename Value>
            class _toPair<std::pair<Key, Value>> {
            protected:
                rpc::driver::IDataFrameId getId() = 0;

            public:
                template<typename Tp>
                std::shared_ptr<IPairDataFrame<Key, Value>> toPair() {
                    return IPairDataFrame<Key, Value>::make(getId());
                }
            };

            template<typename Tp>
            class IDataFrame : public IAbstractDataFrame, public _toPair<Tp> {
            public:
                typedef Tp Tp_type;

                using IAbstractDataFrame::cache;
                using IAbstractDataFrame::persist;
                using IAbstractDataFrame::setName;
                using IAbstractDataFrame::uncache;
                using IAbstractDataFrame::unpersist;

                /*IO*/
                template<typename R>
                std::shared_ptr<IDataFrame<R>> repartition(int64_t numPartitions) {
                    return IDataFrame<R>::make(repartitionAbs(numPartitions));
                }

                template<typename R>
                std::shared_ptr<IDataFrame<R>> coalesce(int64_t numPartitions, bool shuffle) {
                    return IDataFrame<R>::make(coalesceAbs(numPartitions, shuffle));
                }

                using IAbstractDataFrame::partitions;
                using IAbstractDataFrame::saveAsJsonFile;
                using IAbstractDataFrame::saveAsObjectFile;
                using IAbstractDataFrame::saveAsTextFile;

                /*General*/
                template<typename R>
                std::shared_ptr<IDataFrame<R>> map(const ISource &src) {
                    return IDataFrame<R>::make(mapAbs(src));
                }

                template<typename R>
                std::shared_ptr<IDataFrame<R>> filter(const ISource &src) {
                    return IDataFrame<R>::make(filterAbs(src));
                }

                template<typename R>
                std::shared_ptr<IDataFrame<R>> flatmap(const ISource &src) {
                    return IDataFrame<R>::make(flatmapAbs(src));
                }

                template<typename Key>
                std::shared_ptr<IPairDataFrame<Key, Tp>> keyBy(const ISource &src);

                template<typename R>
                std::shared_ptr<IDataFrame<R>> mapPartitions(const ISource &src, bool preservesPartitioning = true) {
                    return IDataFrame<R>::make(mapPartitionsAbs(src, preservesPartitioning));
                }

                template<typename R>
                std::shared_ptr<IDataFrame<R>> mapPartitionsWithIndex(const ISource &src,
                                                                      bool preservesPartitioning = true) {
                    return IDataFrame<R>::make(mapPartitionsWithIndexAbs(src, preservesPartitioning));
                }

                template<typename R>
                std::shared_ptr<IDataFrame<R>> mapExecutor(const ISource &src) {
                    return IDataFrame<R>::make(mapExecutorAbs(src));
                }

                template<typename R>
                std::shared_ptr<IDataFrame<R>> mapExecutorTo(const ISource &src) {
                    return IDataFrame<R>::make(mapExecutorAbsTo(src));
                }

                template<typename Key>
                std::shared_ptr<IPairDataFrame<Key, executor::api::IVector<Tp>>> groupBy(const ISource &src);

                template<typename Key>
                std::shared_ptr<IPairDataFrame<Key, executor::api::IVector<Tp>>> groupBy(const ISource &src,
                                                                                         int64_t numPartitions);

                std::shared_ptr<IDataFrame<Tp>> sort(bool ascending = true) {
                    return IDataFrame<Tp>::make(sortAbs(ascending));
                }

                std::shared_ptr<IDataFrame<Tp>> sort(bool ascending, int64_t numPartitions) {
                    return IDataFrame<Tp>::make(sortAbs(ascending, numPartitions));
                }

                std::shared_ptr<IDataFrame<Tp>> sortBy(const ISource &src, bool ascending) {
                    return IDataFrame<Tp>::make(sortByAbs(src, ascending));
                }

                std::shared_ptr<IDataFrame<Tp>> sortBy(const ISource &src, bool ascending, int64_t numPartitions) {
                    return IDataFrame<Tp>::make(sortByAbs(src, ascending, numPartitions));
                }

                std::shared_ptr<IDataFrame<Tp>> _union(const std::shared_ptr<IDataFrame<Tp>> &other,
                                                       bool preserveOrder) {
                    return IDataFrame<Tp>::make(unionAbs(other->id, preserveOrder));
                }

                std::shared_ptr<IDataFrame<Tp>> _union(const std::shared_ptr<IDataFrame<Tp>> &other, bool preserveOrder,
                                                       int64_t numPartitions) {
                    return IDataFrame<Tp>::make(unionAbs(other->id, preserveOrder, numPartitions));
                }

                std::shared_ptr<IDataFrame<Tp>> _union(const std::shared_ptr<IDataFrame<Tp>> &other, bool preserveOrder,
                                                       const ISource &src) {
                    return IDataFrame<Tp>::make(unionAbs(other->id, preserveOrder, src));
                }

                std::shared_ptr<IDataFrame<Tp>> _union(const std::shared_ptr<IDataFrame<Tp>> &other, bool preserveOrder,
                                                       int64_t numPartitions, const ISource &src) {
                    return IDataFrame<Tp>::make(unionAbs(other->id, preserveOrder, numPartitions, src));
                }

                std::shared_ptr<IDataFrame<Tp>> distinct() { return IDataFrame<Tp>::make(distinctAbs()); }

                std::shared_ptr<IDataFrame<Tp>> distinct(int64_t numPartitions) {
                    return IDataFrame<Tp>::make(distinctAbs(numPartitions));
                }

                std::shared_ptr<IDataFrame<Tp>> distinct(const ISource &src) {
                    return IDataFrame<Tp>::make(distinctAbs(src));
                }

                std::shared_ptr<IDataFrame<Tp>> distinct(int64_t numPartitions, const ISource &src) {
                    return IDataFrame<Tp>::make(distinctAbs(numPartitions, src));
                }

                /*General Action*/
                Tp reduce(const ISource &src) {
                    auto tp = driverContext().template registerType<Tp>();
                    return driverContext().template collect1<Tp>(reduceAbs(src, tp));
                }

                Tp treeReduce(const ISource &src) {
                    auto tp = driverContext().template registerType<Tp>();
                    return driverContext().template collect1<Tp>(treeReduceAbs(src, tp));
                }

                executor::api::IVector<Tp> collect() {
                    auto tp = driverContext().template registerType<Tp>();
                    return driverContext().template collect<Tp>(collectAbs(tp));
                }

                Tp aggregate(const ISource &zero, const ISource &seqOp, const ISource &combOp) {
                    auto tp = driverContext().template registerType<Tp>();
                    return driverContext().template collect1<Tp>(aggregateAbs(zero, seqOp, combOp, tp));
                }

                Tp treeAggregate(const ISource &zero, const ISource &seqOp, const ISource &combOp) {
                    auto tp = driverContext().template registerType<Tp>();
                    return driverContext().template collect1<Tp>(treeAggregateAbs(zero, seqOp, combOp, tp));
                }

                Tp fold(const ISource &zero, const ISource &src) {
                    auto tp = driverContext().template registerType<Tp>();
                    return driverContext().template collect1<Tp>(foldAbs(zero, src, tp));
                }

                Tp treeFold(const ISource &zero, const ISource &src) {
                    auto tp = driverContext().template registerType<Tp>();
                    return driverContext().template collect1<Tp>(treeFoldAbs(zero, src, tp));
                }

                executor::api::IVector<Tp> take(int64_t num) {
                    auto tp = driverContext().template registerType<Tp>();
                    return driverContext().template collect<Tp>(takeAbs(num, tp), num);
                }

                using IAbstractDataFrame::foreach;
                using IAbstractDataFrame::foreachExecutor;
                using IAbstractDataFrame::foreachPartition;

                executor::api::IVector<Tp> top(int64_t num) {
                    auto tp = driverContext().template registerType<Tp>();
                    return driverContext().template collect<Tp>(topAbs(num, tp), num);
                }

                executor::api::IVector<Tp> top(int64_t num, const ISource &cmp) {
                    auto tp = driverContext().template registerType<Tp>();
                    return driverContext().collect(topAbs(num, cmp, tp), num);
                }

                executor::api::IVector<Tp> takeOrdered(int64_t num) {
                    auto tp = driverContext().template registerType<Tp>();
                    return driverContext().template collect<Tp>(takeOrderedAbs(num, tp), num);
                }

                executor::api::IVector<Tp> takeOrdered(int64_t num, const ISource &cmp) {
                    auto tp = driverContext().template registerType<Tp>();
                    return driverContext().collect(takeOrderedAbs(num, cmp, tp), num);
                }

                /*Math*/
                std::shared_ptr<IDataFrame<Tp>> sample(bool withReplacement, double fraction, int seed) {
                    return IDataFrame<Tp>::make(sampleAbs(withReplacement, fraction, seed));
                }

                executor::api::IVector<Tp> takeSample(bool withReplacement, int64_t num, int seed) {
                    auto tp = driverContext().template registerType<Tp>();
                    return driverContext().template collect<Tp>(takeSampleAbs(withReplacement, num, seed, tp));
                };

                using IAbstractDataFrame::count;

                Tp max() {
                    auto tp = driverContext().template registerType<Tp>();
                    return driverContext().template collect<Tp>(maxAbs(tp));
                }

                Tp max(const ISource &cmp) {
                    auto tp = driverContext().template registerType<Tp>();
                    return driverContext().template collect<Tp>(maxAbs(cmp, tp));
                }

                Tp min() {
                    auto tp = driverContext().template registerType<Tp>();
                    return driverContext().template collect<Tp>(minAbs(tp));
                }

                Tp min(const ISource &cmp) {
                    auto tp = driverContext().template registerType<Tp>();
                    return driverContext().template collect<Tp>(minAbs(cmp, tp));
                }

            private:
                friend IWorker;

                IDataFrame(const rpc::driver::IDataFrameId &id) : IAbstractDataFrame(id) {}

                inline static std::shared_ptr<IDataFrame<Tp>> make(const rpc::driver::IDataFrameId &id) {
                    return std::shared_ptr<IDataFrame<Tp>>(new IDataFrame(id));
                }
            };

            template<typename Key, typename Value>
            class IPairDataFrame : public IDataFrame<std::pair<Key, Value>> {
            public:
                typedef Key Key_type;
                typedef Value Value_type;

                IPairDataFrame() = delete;

                std::shared_ptr<IPairDataFrame<Key, Value>>
                join(const std::shared_ptr<IPairDataFrame<Key, Value>> &other) {
                    return IPairDataFrame<Key, Value>::make(this->joinAbs(other->id));
                }

                std::shared_ptr<IPairDataFrame<Key, Value>>
                join(const std::shared_ptr<IPairDataFrame<Key, Value>> &other, int64_t numPartitions) {
                    return IPairDataFrame<Key, Value>::make(this->joinAbs(other->id, numPartitions));
                }

                std::shared_ptr<IPairDataFrame<Key, Value>>
                join(const std::shared_ptr<IPairDataFrame<Key, Value>> &other, const ISource &src) {
                    return IPairDataFrame<Key, Value>::make(this->joinAbs(other->id, src));
                }

                std::shared_ptr<IPairDataFrame<Key, Value>>
                join(const std::shared_ptr<IPairDataFrame<Key, Value>> &other, int64_t numPartitions,
                     const ISource &src) {
                    return IPairDataFrame<Key, Value>::make(this->joinAbs(other->id, numPartitions, src));
                }

                template<typename R>
                std::shared_ptr<IPairDataFrame<Key, R>> flatMapValues(const ISource &src) {
                    return IPairDataFrame<Key, R>::make(this->flatMapValuesAbs(src));
                }

                template<typename R>
                std::shared_ptr<IPairDataFrame<Key, R>> mapValues(const ISource &src) {
                    return IPairDataFrame<Key, R>::make(this->mapValuesAbs(src));
                }

                std::shared_ptr<IPairDataFrame<Key, executor::api::IVector<Value>>> groupByKey() {
                    return IPairDataFrame<Key, executor::api::IVector<Value>>::make(this->groupByKeyAbs());
                }

                std::shared_ptr<IPairDataFrame<Key, executor::api::IVector<Value>>> groupByKey(int64_t numPartitions) {
                    return IPairDataFrame<Key, executor::api::IVector<Value>>::make(this->groupByKeyAbs(numPartitions));
                }

                std::shared_ptr<IPairDataFrame<Key, executor::api::IVector<Value>>> groupByKey(const ISource &src) {
                    return IPairDataFrame<Key, executor::api::IVector<Value>>::make(this->groupByKeyAbs(src));
                }

                std::shared_ptr<IPairDataFrame<Key, executor::api::IVector<Value>>> groupByKey(int64_t numPartitions,
                                                                                               const ISource &src) {
                    return IPairDataFrame<Key, executor::api::IVector<Value>>::make(
                            this->groupByKeyAbs(numPartitions, src));
                }

                std::shared_ptr<IPairDataFrame<Key, Value>> reduceByKey(const ISource &src, bool localReduce = true) {
                    return IPairDataFrame<Key, Value>::make(this->reduceByKeyAbs(src, localReduce));
                }

                std::shared_ptr<IPairDataFrame<Key, Value>> reduceByKey(const ISource &src, int64_t numPartitions,
                                                                        bool localReduce = true) {
                    return IPairDataFrame<Key, Value>::make(this->reduceByKeyAbs(src, numPartitions, localReduce));
                }

                template<typename R>
                std::shared_ptr<IPairDataFrame<Key, R>> aggregateByKey(const ISource &zero, const ISource &seqOp) {
                    return IPairDataFrame<Key, R>::make(this->aggregateByKeyAbs(zero, seqOp));
                }

                template<typename R>
                std::shared_ptr<IPairDataFrame<Key, R>> aggregateByKey(const ISource &zero, const ISource &seqOp,
                                                                       int64_t numPartitions) {
                    return IPairDataFrame<Key, R>::make(this->aggregateByKeyAbs(zero, seqOp, numPartitions));
                }

                template<typename R>
                std::shared_ptr<IPairDataFrame<Key, R>> aggregateByKey(const ISource &zero, const ISource &seqOp,
                                                                       const ISource &combOp) {
                    return IPairDataFrame<Key, R>::make(this->aggregateByKeyAbs(zero, seqOp, combOp));
                }

                template<typename R>
                std::shared_ptr<IPairDataFrame<Key, R>> aggregateByKey(const ISource &zero, const ISource &seqOp,
                                                                       const ISource &combOp, int64_t numPartitions) {
                    return IPairDataFrame<Key, R>::make(this->aggregateByKeyAbs(zero, seqOp, combOp, numPartitions));
                }


                std::shared_ptr<IPairDataFrame<Key, Value>> foldByKey(const ISource &zero, const ISource &src,
                                                                      bool localFold = true) {
                    return IPairDataFrame<Key, Value>::make(this->foldByKeyAbs(zero, src, localFold));
                }


                std::shared_ptr<IPairDataFrame<Key, Value>> foldByKey(const ISource &zero, const ISource &src,
                                                                      int64_t numPartitions, bool localFold = true) {
                    return IPairDataFrame<Key, Value>::make(this->foldByKeyAbs(zero, src, numPartitions, localFold));
                }

                std::shared_ptr<IPairDataFrame<Key, Value>> sortByKey(bool ascending = true) {
                    return IPairDataFrame<Key, Value>::make(this->sortByKeyAbs(ascending));
                }

                std::shared_ptr<IPairDataFrame<Key, Value>> sortByKey(bool ascending, int64_t numPartitions) {
                    return IPairDataFrame<Key, Value>::make(this->sortByKeyAbs(ascending, numPartitions));
                }

                std::shared_ptr<IPairDataFrame<Key, Value>> sortByKey(const ISource &src, bool ascending = true) {
                    return IPairDataFrame<Key, Value>::make(this->sortByKeyAbs(src, ascending));
                }

                std::shared_ptr<IPairDataFrame<Key, Value>> sortByKey(const ISource &src, bool ascending,
                                                                      int64_t numPartitions) {
                    return IPairDataFrame<Key, Value>::make(this->sortByKeyAbs(src, ascending, numPartitions));
                }

                executor::api::IVector<Key> keys() {
                    auto tp = this->driverContext().template registerType<Key>();
                    return this->driverContext().template collect<Key>(keysAbs(tp));
                }

                executor::api::IVector<Value> values() {
                    auto tp = this->driverContext().template registerType<Value>();
                    return this->driverContext().template collect<Value>(valuesAbs(tp));
                }

                std::shared_ptr<IPairDataFrame<Key, Value>>
                sampleByKey(bool withReplacement, const std::map<Key, int64_t> &fractions, int seed) {
                    ISource fractions_src("");
                    fractions_src.addParam("fractions", fractions);
                    return IPairDataFrame<Key, Value>::make(this->sampleByKeyAbs(withReplacement, fractions, seed));
                }

                std::shared_ptr<IPairDataFrame<Key, Value>>
                sampleByKey(bool withReplacement, const std::unordered_map<Key, int64_t> &fractions, int seed) {
                    ISource fractions_src("");
                    fractions_src.addParam("fractions", fractions);
                    return IPairDataFrame<Key, Value>::make(this->sampleByKeyAbs(withReplacement, fractions_src, seed));
                }

                std::unordered_map<Key, int64_t> countByKey() {
                    auto tp = this->driverContext().template registerType<std::unordered_map<Key, int64_t>>();
                    auto counts =
                            this->driverContext().template collect<std::unordered_map<Key, int64_t>>(countByKeyAbs(tp));
                    for (int64_t i = 1; i < counts.size(); i++) {
                        for (auto &item : counts[i]) { counts[0][item.first] += item.second; }
                    }
                    return counts[0];
                }

                std::unordered_map<Value, int64_t> countByValue() {
                    auto tp = this->driverContext().template registerType<std::unordered_map<Value, int64_t>>();
                    auto counts = this->driverContext().template collect<std::unordered_map<Value, int64_t>>(
                            countByKeyAbs(tp));
                    for (int64_t i = 1; i < counts.size(); i++) {
                        for (auto &item : counts[i]) { counts[0][item.first] += item.second; }
                    }
                    return counts[0];
                }

            private:
                friend IDataFrame<std::pair<Key, Value>>;

                inline static std::shared_ptr<IPairDataFrame<Key, Value>> make(const rpc::driver::IDataFrameId &id) {
                    return std::shared_ptr<IPairDataFrame<Key, Value>>(new IDataFrame<std::pair<Key, Value>>(id));
                }
            };

            template<typename Tp>
            template<typename Key>
            std::shared_ptr<IPairDataFrame<Key, Tp>> IDataFrame<Tp>::keyBy(const ISource &src) {
                return IPairDataFrame<Key, Tp>::make(keyByAbs(src));
            }

            template<typename Tp>
            template<typename Key>
            std::shared_ptr<IPairDataFrame<Key, executor::api::IVector<Tp>>>
            IDataFrame<Tp>::groupBy(const ISource &src) {
                return IPairDataFrame<Key, executor::api::IVector<Tp>>::make(groupByAbs(src));
            }

            template<typename Tp>
            template<typename Key>
            std::shared_ptr<IPairDataFrame<Key, executor::api::IVector<Tp>>>
            IDataFrame<Tp>::groupBy(const ISource &src, int64_t numPartitions) {
                return IPairDataFrame<Key, executor::api::IVector<Tp>>::make(groupByAbs(src, numPartitions));
            }

        }// namespace api
    }    // namespace driver
}// namespace ignis

#endif
