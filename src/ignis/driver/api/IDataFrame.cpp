
#include "IDataFrame.h"
#include "IDriverException.h"
#include "ignis/rpc/driver/IDriverException_types.h"

using namespace ignis::driver::api;
using ignis::rpc::driver::IDataFrameId;

ignis::rpc::driver::IDataFrameId IAbstractDataFrame::getId() { return this->id; }

IAbstractDataFrame::IAbstractDataFrame(const IDataFrameId &id) : id(id) {}

ignis::driver::core::IDriverContext &IAbstractDataFrame::driverContext() { return Ignis::driverContext(); }

void IAbstractDataFrame::setName(const std::string &name) {
    try {
        Ignis::clientPool->getClient()->getDataFrameService().setName(id, name);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

void IAbstractDataFrame::persist(const ICacheLevel &level) {
    try {
        Ignis::clientPool->getClient()->getDataFrameService().persist(id, level);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

void IAbstractDataFrame::cache() {
    try {
        Ignis::clientPool->getClient()->getDataFrameService().cache(id);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

void IAbstractDataFrame::unpersist() {
    try {
        Ignis::clientPool->getClient()->getDataFrameService().unpersist(id);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

void IAbstractDataFrame::uncache() {
    try {
        Ignis::clientPool->getClient()->getDataFrameService().uncache(id);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::repartitionAbs(int64_t numPartitions) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().repartition(_return, id, numPartitions);
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::coalesceAbs(int64_t numPartitions, bool shuffle) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().coalesce(_return, id, numPartitions, shuffle);
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

int64_t IAbstractDataFrame::partitions() {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().partitions(id);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

void IAbstractDataFrame::saveAsObjectFile(const std::string &path, int compression) {
    try {
        Ignis::clientPool->getClient()->getDataFrameService().saveAsObjectFile(id, path, compression);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

void IAbstractDataFrame::saveAsTextFile(const std::string &path) {
    try {
        Ignis::clientPool->getClient()->getDataFrameService().saveAsTextFile(id, path);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

void IAbstractDataFrame::saveAsJsonFile(const std::string &path, bool pretty) {
    try {
        Ignis::clientPool->getClient()->getDataFrameService().saveAsJsonFile(id, path, pretty);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::mapAbs(const ISource &src) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().map_(_return, id, src.rpc());
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::filterAbs(const ISource &src) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().filter(_return, id, src.rpc());
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::flatmapAbs(const ISource &src) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().flatmap(_return, id, src.rpc());
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::keyByAbs(const ISource &src) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().keyBy(_return, id, src.rpc());
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::mapPartitionsAbs(const ISource &src, bool preservesPartitioning) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().mapPartitions(_return, id, src.rpc(),
                                                                            preservesPartitioning);
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::mapPartitionsWithIndexAbs(const ISource &src, bool preservesPartitioning) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().mapPartitionsWithIndex(_return, id, src.rpc(),
                                                                                     preservesPartitioning);
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::mapExecutorAbs(const ISource &src) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().mapExecutor(_return, id, src.rpc());
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::mapExecutorAbsTo(const ISource &src) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().mapExecutorTo(_return, id, src.rpc());
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::groupByAbs(const ISource &src) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().groupBy(_return, id, src.rpc());
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::groupByAbs(const ISource &src, int64_t numPartitions) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().groupBy2(_return, id, src.rpc(), numPartitions);
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::sortAbs(bool ascending) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().sort(_return, id, ascending);
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::sortAbs(bool ascending, int64_t numPartitions) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().sort2(_return, id, ascending, numPartitions);
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::sortByAbs(const ISource &src, bool ascending) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().sortBy(_return, id, src.rpc(), ascending);
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::sortByAbs(const ISource &src, bool ascending, int64_t numPartitions) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().sortBy3(_return, id, src.rpc(), ascending, numPartitions);
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::unionAbs(const rpc::driver::IDataFrameId &other, bool preserveOrder) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().union_(_return, id, other, preserveOrder);
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::unionAbs(const rpc::driver::IDataFrameId &other, bool preserveOrder,
                                        int64_t numPartitions) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().union4a(_return, id, other, preserveOrder, numPartitions);
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::unionAbs(const rpc::driver::IDataFrameId &other, bool preserveOrder,
                                        const ISource &src) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().union4b(_return, id, other, preserveOrder, src.rpc());
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::unionAbs(const rpc::driver::IDataFrameId &other, bool preserveOrder,
                                        int64_t numPartitions, const ISource &src) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().union5(_return, id, other, preserveOrder, numPartitions,
                                                                     src.rpc());
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::distinctAbs() {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().distinct(_return, id);
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::distinctAbs(int64_t numPartitions) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().distinct2a(_return, id, numPartitions);
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::distinctAbs(const ISource &src) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().distinct2b(_return, id, src.rpc());
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::distinctAbs(int64_t numPartitions, const ISource &src) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().distinct3(_return, id, numPartitions, src.rpc());
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

int64_t IAbstractDataFrame::reduceAbs(const ISource &src, const ignis::rpc::ISource &tp) {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().reduce(id, src.rpc(), tp);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

int64_t IAbstractDataFrame::treeReduceAbs(const ISource &src, const ignis::rpc::ISource &tp) {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().treeReduce(id, src.rpc(), tp);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

int64_t IAbstractDataFrame::collectAbs(const ignis::rpc::ISource &tp) {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().collect(id, tp);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

int64_t IAbstractDataFrame::aggregateAbs(const ISource &zero, const ISource &seqOp, const ISource &combOp,
                                         const ignis::rpc::ISource &tp) {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().aggregate(id, zero.rpc(), seqOp.rpc(),
                                                                               combOp.rpc(), tp);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

int64_t IAbstractDataFrame::treeAggregateAbs(const ISource &zero, const ISource &seqOp, const ISource &combOp,
                                             const ignis::rpc::ISource &tp) {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().treeAggregate(id, zero.rpc(), seqOp.rpc(),
                                                                                   combOp.rpc(), tp);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

int64_t IAbstractDataFrame::foldAbs(const ISource &zero, const ISource &src, const ignis::rpc::ISource &tp) {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().fold(id, zero.rpc(), src.rpc(), tp);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

int64_t IAbstractDataFrame::treeFoldAbs(const ISource &zero, const ISource &src, const ignis::rpc::ISource &tp) {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().treeFold(id, zero.rpc(), src.rpc(), tp);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

int64_t IAbstractDataFrame::takeAbs(int64_t num, const ignis::rpc::ISource &tp) {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().take(id, num, tp);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

void IAbstractDataFrame::foreach (const ISource &src) {
    try {
        Ignis::clientPool->getClient()->getDataFrameService().foreach_(id, src.rpc());
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

void IAbstractDataFrame::foreachPartition(const ISource &src) {
    try {
        Ignis::clientPool->getClient()->getDataFrameService().foreachPartition(id, src.rpc());
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

void IAbstractDataFrame::foreachExecutor(const ISource &src) {
    try {
        Ignis::clientPool->getClient()->getDataFrameService().foreachExecutor(id, src.rpc());
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

int64_t IAbstractDataFrame::topAbs(int64_t num, const ignis::rpc::ISource &tp) {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().top(id, num, tp);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

int64_t IAbstractDataFrame::topAbs(int64_t num, const ISource &cmp, const ignis::rpc::ISource &tp) {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().top4(id, num, cmp.rpc(), tp);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

int64_t IAbstractDataFrame::takeOrderedAbs(int64_t num, const ignis::rpc::ISource &tp) {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().takeOrdered(id, num, tp);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

int64_t IAbstractDataFrame::takeOrderedAbs(int64_t num, const ISource &cmp, const ignis::rpc::ISource &tp) {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().takeOrdered4(id, num, cmp.rpc(), tp);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::sampleAbs(bool withReplacement, double fraction, int seed) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().sample(_return, id, withReplacement, fraction, seed);
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

int64_t IAbstractDataFrame::takeSampleAbs(bool withReplacement, int64_t num, int seed, const ignis::rpc::ISource &tp) {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().takeSample(id, withReplacement, num, seed, tp);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

int64_t IAbstractDataFrame::count() {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().count(id);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

int64_t IAbstractDataFrame::maxAbs(const ignis::rpc::ISource &tp) {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().max(id, tp);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

int64_t IAbstractDataFrame::maxAbs(const ISource &cmp, const ignis::rpc::ISource &tp) {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().max3(id, cmp.rpc(), tp);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

int64_t IAbstractDataFrame::minAbs(const ignis::rpc::ISource &tp) {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().min(id, tp);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

int64_t IAbstractDataFrame::minAbs(const ISource &cmp, const ignis::rpc::ISource &tp) {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().min3(id, cmp.rpc(), tp);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::joinAbs(const rpc::driver::IDataFrameId &other) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().join(_return, id, other);
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::joinAbs(const rpc::driver::IDataFrameId &other, int64_t numPartitions) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().join3a(_return, id, other, numPartitions);
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::joinAbs(const rpc::driver::IDataFrameId &other, const ISource &src) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().join3b(_return, id, other, src.rpc());
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::joinAbs(const rpc::driver::IDataFrameId &other, int64_t numPartitions,
                                      const ISource &src) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().join4(_return, id, other, numPartitions, src.rpc());
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::flatMapValuesAbs(const ISource &src) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().flatMapValues(_return, id, src.rpc());
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::mapValuesAbs(const ISource &src) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().mapValues(_return, id, src.rpc());
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::groupByKeyAbs() {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().groupByKey(_return, id);
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::groupByKeyAbs(int64_t numPartitions) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().groupByKey2a(_return, id, numPartitions);
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::groupByKeyAbs(const ISource &src) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().groupByKey2b(_return, id, src.rpc());
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::groupByKeyAbs(int64_t numPartitions, const ISource &src) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().groupByKey3(_return, id, numPartitions, src.rpc());
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::reduceByKeyAbs(const ISource &src, bool localReduce) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().reduceByKey(_return, id, src.rpc(), localReduce);
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::reduceByKeyAbs(const ISource &src, int64_t numPartitions, bool localReduce) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().reduceByKey4(_return, id, src.rpc(), numPartitions,
                                                                           localReduce);
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::aggregateByKeyAbs(const ISource &zero, const ISource &seqOp) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().aggregateByKey(_return, id, zero.rpc(), seqOp.rpc());
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::aggregateByKeyAbs(const ISource &zero, const ISource &seqOp, int64_t numPartitions) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().aggregateByKey4a(_return, id, zero.rpc(), seqOp.rpc(),
                                                                               numPartitions);
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::aggregateByKeyAbs(const ISource &zero, const ISource &seqOp, const ISource &combOp) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().aggregateByKey4b(_return, id, zero.rpc(), seqOp.rpc(),
                                                                               combOp.rpc());
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::aggregateByKeyAbs(const ISource &zero, const ISource &seqOp, const ISource &combOp,
                                                   int64_t numPartitions) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().aggregateByKey5(_return, id, zero.rpc(), seqOp.rpc(),
                                                                              combOp.rpc(), numPartitions);
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::foldByKeyAbs(const ISource &zero, const ISource &src, bool localFold) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().foldByKey(_return, id, zero.rpc(), src.rpc(), localFold);
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::foldByKeyAbs(const ISource &zero, const ISource &src, int64_t numPartitions,
                                              bool localFold) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().foldByKey5(_return, id, zero.rpc(), src.rpc(),
                                                                         numPartitions, localFold);
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::sortByKeyAbs(bool ascending) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().sortByKey(_return, id, ascending);
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::sortByKeyAbs(bool ascending, int64_t numPartitions) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().sortByKey3a(_return, id, ascending, numPartitions);
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::sortByKeyAbs(const ISource &src, bool ascending) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().sortByKey3b(_return, id, src.rpc(), ascending);
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::sortByKeyAbs(const ISource &src, bool ascending, int64_t numPartitions) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().sortByKey4(_return, id, src.rpc(), ascending,
                                                                         numPartitions);
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

int64_t IAbstractDataFrame::keysAbs(const rpc::ISource &tp) {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().keys(id, tp);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

int64_t IAbstractDataFrame::valuesAbs(const rpc::ISource &tp) {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().keys(id, tp);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IAbstractDataFrame::sampleByKeyAbs(bool withReplacement, const ISource &fractions, int seed) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().sampleByKey(_return, id, withReplacement, fractions.rpc(),
                                                                          seed);
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

int64_t IAbstractDataFrame::countByKeyAbs(const rpc::ISource &tp) {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().countByKey(id, tp);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

int64_t IAbstractDataFrame::countByValueAbs(const rpc::ISource &tp) {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().countByKey(id, tp);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}