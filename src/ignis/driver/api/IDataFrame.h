
#ifndef IGNIS_IDATAFRAME_H
#define IGNIS_IDATAFRAME_H

#include "Ignis.h"
#include "ignis/rpc/driver/IDataFrameService_types.h"
#include "ignis/rpc/ISource_types.h"
#include <vector>
#include "ISource.h"

namespace ignis {
    namespace driver {
        namespace api {

            enum ICacheLevel {
                NO_CACHE = 0,
                PRESERVE = 1,
                MEMORY = 2,
                RAW_MEMORY = 3,
                DISK = 4
            };

            class IAbstractDataFrame {
            protected:
                rpc::driver::IDataFrameId id;

                IAbstractDataFrame(const rpc::driver::IDataFrameId &id);

                core::IDriverContext &driverPartition();

                void setName(const std::string &name);

                void persist(const ICacheLevel &level);

                void cache();

                void unpersist();

                void uncache();

                /*IO*/
                rpc::driver::IDataFrameId repartitionAbs(int64_t numPartitions);

                rpc::driver::IDataFrameId coalesceAbs(int64_t numPartitions, bool shuffle);

                int64_t partitions();

                void saveAsObjectFile(const std::string &path, int compression);

                void saveAsTextFile(const std::string &path);

                void saveAsJsonFile(const std::string &path, bool pretty = true);

                /*General*/
                rpc::driver::IDataFrameId mapAbs(const ISource &src);

                rpc::driver::IDataFrameId filterAbs(const ISource &src);

                rpc::driver::IDataFrameId flatmapAbs(const ISource &src);

                rpc::driver::IDataFrameId mapPartitionsAbs(const ISource &src, bool preservesPartitioning);

                rpc::driver::IDataFrameId mapPartitionsWithIndexAbs(const ISource &src, bool preservesPartitioning);

                rpc::driver::IDataFrameId applyPartitionAbs(const ISource &src);

                rpc::driver::IDataFrameId groupByAbs(const ISource &src);

                rpc::driver::IDataFrameId groupByAbs(const ISource &src, int64_t numPartitions);

                rpc::driver::IDataFrameId sortAbs(bool ascending);

                rpc::driver::IDataFrameId sortAbs(bool ascending, int64_t numPartitions);

                rpc::driver::IDataFrameId sortByAbs(const ISource &src, bool ascending);

                rpc::driver::IDataFrameId sortByAbs(const ISource &src, bool ascending, int64_t numPartitions);

                /*General Action*/
                int64_t reduceAbs(const ISource &src, const rpc::ISource &tp);

                int64_t treeReduceAbs(const ISource &src, const rpc::ISource &tp);

                int64_t treeReduceAbs(const ISource &src, int64_t depth, const rpc::ISource &tp);

                int64_t collectAbs(const rpc::ISource &tp);

                int64_t aggregateAbs(const ISource &seqOp, const ISource &combOp, const rpc::ISource &tp);

                int64_t treeAggregateAbs(const ISource &seqOp, const ISource &combOp, const rpc::ISource &tp);

                int64_t
                treeAggregateAbs(const ISource &seqOp, const ISource &combOp, int64_t depth, const rpc::ISource &tp);

                int64_t foldAbs(const ISource &src, const rpc::ISource &tp);

                int64_t takeAbs(int64_t num, const rpc::ISource &tp);

                void foreach(const ISource &src);

                void foreachPartition(const ISource &src);

                int64_t topAbs(int64_t num, const rpc::ISource &tp);

                int64_t topAbs(int64_t num, const ISource &cmp, const rpc::ISource &tp);

                /*Math*/
                rpc::driver::IDataFrameId sampleAbs(bool withReplacement, double fraction, int seed);

                int64_t takeSampleAbs(bool withReplacement, int64_t num, int seed, const rpc::ISource &tp);

                int64_t count();

                int64_t maxAbs(const ISource &cmp, const rpc::ISource &tp);

                int64_t minAbs(const ISource &cmp, const rpc::ISource &tp);

            };

            class IWorker;

            template<typename Tp>
            class IDataFrame : private IAbstractDataFrame {
            public:

                using IAbstractDataFrame::setName;
                using IAbstractDataFrame::persist;
                using IAbstractDataFrame::cache;
                using IAbstractDataFrame::unpersist;
                using IAbstractDataFrame::uncache;

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
                using IAbstractDataFrame::saveAsObjectFile;
                using IAbstractDataFrame::saveAsTextFile;
                using IAbstractDataFrame::saveAsJsonFile;

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

                template<typename R>
                std::shared_ptr<IDataFrame<R>> mapPartitions(const ISource &src, bool preservesPartitioning) {
                    return IDataFrame<R>::make(mapPartitionsAbs(src, preservesPartitioning));
                }

                template<typename R>
                std::shared_ptr<IDataFrame<R>> mapPartitionsWithIndex(const ISource &src, bool preservesPartitioning) {
                    return IDataFrame<R>::make(mapPartitionsWithIndexAbs(src, preservesPartitioning));
                }

                template<typename R>
                std::shared_ptr<IDataFrame<R>> applyPartition(const ISource &src) {
                    return IDataFrame<R>::make(applyPartitionAbs(src));
                }

                template<typename R>
                std::shared_ptr<IDataFrame<R>> groupBy(const ISource &src) {
                    return IDataFrame<R>::make(groupByAbs(src));
                }

                template<typename R>
                std::shared_ptr<IDataFrame<R>> groupBy(const ISource &src, int64_t numPartitions) {
                    return IDataFrame<R>::make(groupByAbs(src, numPartitions));
                }

                std::shared_ptr<IDataFrame<Tp>> sort(bool ascending=true) {
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

                /*General Action*/
                Tp reduce(const ISource &src) {
                    auto tp = driverPartition().template registerType<Tp>();
                    return driverPartition().template collect1<Tp>(reduceAbs(src, tp));
                }

                Tp treeReduce(const ISource &src) {
                    auto tp = driverPartition().template registerType<Tp>();
                    return driverPartition().template collect1<Tp>(treeReduceAbs(src, tp));
                }

                Tp treeReduce(const ISource &src, int64_t depth) {
                    auto tp = driverPartition().template registerType<Tp>();
                    return driverPartition().template collect1<Tp>(treeReduceAbs(src, depth, tp));
                }

                std::vector<Tp> collect() {
                    auto tp = driverPartition().template registerType<Tp>();
                    return driverPartition().template collect<Tp>(collectAbs(tp));
                }

                Tp aggregate(const ISource &seqOp, const ISource &combOp) {
                    auto tp = driverPartition().template registerType<Tp>();
                    return driverPartition().template collect1<Tp>(aggregateAbs(seqOp, combOp, tp));
                }

                Tp treeAggregate(const ISource &seqOp, const ISource &combOp) {
                    auto tp = driverPartition().template registerType<Tp>();
                    return driverPartition().template collect1<Tp>(treeAggregateAbs(seqOp, combOp, tp));
                }

                Tp treeAggregate(const ISource &seqOp, const ISource &combOp, int64_t depth) {
                    auto tp = driverPartition().template registerType<Tp>();
                    return driverPartition().template collect1<Tp>(treeAggregateAbs(seqOp, combOp, depth, tp));
                }

                Tp fold(const ISource &src) {
                    auto tp = driverPartition().template registerType<Tp>();
                    return driverPartition().template collect1<Tp>(foldAbs(src, tp));
                }

                Tp take(int64_t num) {
                    auto tp = driverPartition().template registerType<Tp>();
                    return driverPartition().template collect1<Tp>(takeAbs(num, tp));
                }

                using IAbstractDataFrame::foreach;
                using IAbstractDataFrame::foreachPartition;

                Tp top(int64_t num) {
                    auto tp = driverPartition().template registerType<Tp>();
                    return driverPartition().template collect1<Tp>(topAbs(num, tp));
                }

                Tp top(int64_t num, const ISource &cmp) {
                    auto tp = driverPartition().template registerType<Tp>();
                    return driverPartition().collect1(topAbs(num, cmp, tp));
                }

                /*Math*/
                std::shared_ptr<IDataFrame<Tp>> sample(bool withReplacement, double fraction, int seed) {
                    return IDataFrame<Tp>::make(sampleAbs(withReplacement, fraction, seed));
                }

                std::vector<Tp> takeSample(bool withReplacement, int64_t num, int seed) {
                    auto tp = driverPartition().template registerType<Tp>();
                    return driverPartition().template collect<Tp>(takeSampleAbs(withReplacement, num, seed, tp));
                };

                using IAbstractDataFrame::count;

                Tp max(const ISource &cmp) {
                    auto tp = driverPartition().template registerType<Tp>();
                    return driverPartition().template collect<Tp>(maxAbs(cmp, tp));
                }

                Tp min(const ISource &cmp) {
                    auto tp = driverPartition().template registerType<Tp>();
                    return driverPartition().template collect<Tp>(minAbs(cmp, tp));
                }

            private:
                friend IWorker;

                IDataFrame(const rpc::driver::IDataFrameId &id) : IAbstractDataFrame(id) {}

                static std::shared_ptr<IDataFrame<Tp>> make(const rpc::driver::IDataFrameId &id) {
                    return std::shared_ptr<IDataFrame<Tp>>(new IDataFrame<Tp>(id));
                }

            };
        }
    }
}

#endif
