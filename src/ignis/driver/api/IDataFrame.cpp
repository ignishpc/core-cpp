
#include "IDataFrame.h"
#include "IDriverException.h"
#include "ignis/rpc/driver/IDriverException_types.h"

using namespace ignis::driver::api;
using ignis::rpc::driver::IDataFrameId;

IAbstractDataFrame::IAbstractDataFrame(const IDataFrameId &id) : id(id) {}

ignis::driver::core::IDriverContext &IAbstractDataFrame::driverContext() {
    return Ignis::driverContext();
}

void IAbstractDataFrame::setName(const std::string &name) {
    try {
        Ignis::clientPool->getClient()->getDataFrameService().setName(id, name);
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

void IAbstractDataFrame::persist(const ICacheLevel &level) {
    try {
        Ignis::clientPool->getClient()->getDataFrameService().persist(id, level);
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

void IAbstractDataFrame::cache() {
    try {
        Ignis::clientPool->getClient()->getDataFrameService().cache(id);
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

void IAbstractDataFrame::unpersist() {
    try {
        Ignis::clientPool->getClient()->getDataFrameService().unpersist(id);
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

void IAbstractDataFrame::uncache() {
    try {
        Ignis::clientPool->getClient()->getDataFrameService().uncache(id);
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

IDataFrameId IAbstractDataFrame::repartitionAbs(int64_t numPartitions) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().repartition(_return, id, numPartitions);
        return _return;
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

IDataFrameId IAbstractDataFrame::coalesceAbs(int64_t numPartitions, bool shuffle) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().coalesce(_return, id, numPartitions, shuffle);
        return _return;
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

int64_t IAbstractDataFrame::partitions() {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().partitions(id);
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

void IAbstractDataFrame::saveAsObjectFile(const std::string &path, int compression) {
    try {
        Ignis::clientPool->getClient()->getDataFrameService().saveAsObjectFile(id, path, compression);
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

void IAbstractDataFrame::saveAsTextFile(const std::string &path) {
    try {
        Ignis::clientPool->getClient()->getDataFrameService().saveAsTextFile(id, path);
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

void IAbstractDataFrame::saveAsJsonFile(const std::string &path, bool pretty) {
    try {
        Ignis::clientPool->getClient()->getDataFrameService().saveAsJsonFile(id, path, pretty);
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

IDataFrameId IAbstractDataFrame::mapAbs(const ISource &src) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().map_(_return, id, src.rpc());
        return _return;
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

IDataFrameId IAbstractDataFrame::filterAbs(const ISource &src) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().filter(_return, id, src.rpc());
        return _return;
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

IDataFrameId IAbstractDataFrame::flatmapAbs(const ISource &src) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().flatmap(_return, id, src.rpc());
        return _return;
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

IDataFrameId IAbstractDataFrame::mapPartitionsAbs(const ISource &src, bool preservesPartitioning) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().mapPartitions(_return, id, src.rpc(),
                                                                            preservesPartitioning);
        return _return;
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

IDataFrameId IAbstractDataFrame::mapPartitionsWithIndexAbs(const ISource &src, bool preservesPartitioning) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().mapPartitionsWithIndex(_return, id, src.rpc(),
                                                                                     preservesPartitioning);
        return _return;
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

IDataFrameId IAbstractDataFrame::applyPartitionAbs(const ISource &src) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().applyPartition(_return, id, src.rpc());
        return _return;
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

IDataFrameId IAbstractDataFrame::groupByAbs(const ISource &src) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().groupBy(_return, id, src.rpc());
        return _return;
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

IDataFrameId IAbstractDataFrame::groupByAbs(const ISource &src, int64_t numPartitions) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().groupBy2(_return, id, src.rpc(), numPartitions);
        return _return;
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

IDataFrameId IAbstractDataFrame::sortAbs(bool ascending) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().sort(_return, id, ascending);
        return _return;
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

IDataFrameId IAbstractDataFrame::sortAbs(bool ascending, int64_t numPartitions) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().sort2(_return, id, ascending, numPartitions);
        return _return;
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

IDataFrameId IAbstractDataFrame::sortByAbs(const ISource &src, bool ascending) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().sortBy(_return, id, src.rpc(), ascending);
        return _return;
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

IDataFrameId IAbstractDataFrame::sortByAbs(const ISource &src, bool ascending, int64_t numPartitions) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().sortBy3(_return, id, src.rpc(), ascending,
                                                                      numPartitions);
        return _return;
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

int64_t IAbstractDataFrame::reduceAbs(const ISource &src, const ignis::rpc::ISource &tp) {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().reduce(id, src.rpc(), tp);
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

int64_t IAbstractDataFrame::treeReduceAbs(const ISource &src, const ignis::rpc::ISource &tp) {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().treeReduce(id, src.rpc(), tp);
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

int64_t IAbstractDataFrame::treeReduceAbs(const ISource &src, int64_t depth, const ignis::rpc::ISource &tp) {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().treeReduce4(id, src.rpc(), depth, tp);
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

int64_t IAbstractDataFrame::collectAbs(const ignis::rpc::ISource &tp) {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().collect(id, tp);
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

int64_t IAbstractDataFrame::aggregateAbs(const ISource &seqOp, const ISource &combOp, const ignis::rpc::ISource &tp) {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().aggregate(id, seqOp.rpc(), combOp.rpc(), tp);
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

int64_t
IAbstractDataFrame::treeAggregateAbs(const ISource &seqOp, const ISource &combOp, const ignis::rpc::ISource &tp) {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().treeAggregate(id, seqOp.rpc(), combOp.rpc(), tp);
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

int64_t IAbstractDataFrame::treeAggregateAbs(const ISource &seqOp, const ISource &combOp, int64_t depth,
                                             const ignis::rpc::ISource &tp) {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().treeAggregate5(id, seqOp.rpc(), combOp.rpc(),
                                                                                    depth, tp);
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

int64_t IAbstractDataFrame::foldAbs(const ISource &src, const ignis::rpc::ISource &tp) {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().fold(id, src.rpc(), tp);
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

int64_t IAbstractDataFrame::takeAbs(int64_t num, const ignis::rpc::ISource &tp) {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().take(id, num, tp);
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

void IAbstractDataFrame::foreach(const ISource &src) {
    try {
        Ignis::clientPool->getClient()->getDataFrameService().foreach_(id, src.rpc());
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

void IAbstractDataFrame::foreachPartition(const ISource &src) {
    try {
        Ignis::clientPool->getClient()->getDataFrameService().foreachPartition(id, src.rpc());
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

int64_t IAbstractDataFrame::topAbs(int64_t num, const ignis::rpc::ISource &tp) {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().top(id, num, tp);
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

int64_t IAbstractDataFrame::topAbs(int64_t num, const ISource &cmp, const ignis::rpc::ISource &tp) {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().top4(id, num, cmp.rpc(), tp);
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

IDataFrameId IAbstractDataFrame::sampleAbs(bool withReplacement, double fraction, int seed) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getDataFrameService().sample(_return, id, withReplacement, fraction, seed);
        return _return;
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

int64_t IAbstractDataFrame::takeSampleAbs(bool withReplacement, int64_t num, int seed, const ignis::rpc::ISource &tp) {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().takeSample(id, withReplacement, num, seed, tp);
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

int64_t IAbstractDataFrame::count() {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().count(id);
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

int64_t IAbstractDataFrame::maxAbs(const ISource &cmp, const ignis::rpc::ISource &tp) {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().max(id, cmp.rpc(), tp);
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

int64_t IAbstractDataFrame::minAbs(const ISource &cmp, const ignis::rpc::ISource &tp) {
    try {
        return Ignis::clientPool->getClient()->getDataFrameService().min(id, cmp.rpc(), tp);
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}
