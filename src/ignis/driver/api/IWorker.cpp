
#include "IWorker.h"
#include "IDriverException.h"
#include "ignis/rpc/driver/IDriverException_types.h"

using namespace ignis::driver::api;
using ignis::rpc::driver::IDataFrameId;

IWorker::IWorker(const ICluster &cluster, const std::string &type) : cluster(cluster) {
    try {
        Ignis::clientPool->getClient()->getWorkerService().newInstance(id, cluster.id, type);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IWorker::IWorker(const ICluster &cluster, const std::string &name, const std::string &type)
    : cluster(cluster) {
    try {
        Ignis::clientPool->getClient()->getWorkerService().newInstance3(id, cluster.id, name, type);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IWorker::IWorker(const ICluster &cluster, const std::string &type, int cores, int instances)
    : cluster(cluster) {
    try {
        Ignis::clientPool->getClient()->getWorkerService().newInstance4(id, cluster.id, type, cores, instances);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IWorker::IWorker(const ICluster &cluster, const std::string &name, const std::string &type, int cores,
                 int instances)
    : cluster(cluster) {
    try {
        Ignis::clientPool->getClient()->getWorkerService().newInstance5(id, cluster.id, name, type, cores, instances);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

ICluster IWorker::getCluster() { return cluster; }

void IWorker::start() {
    try {
        Ignis::clientPool->getClient()->getWorkerService().start(id);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

void IWorker::destroy() {
    try {
        Ignis::clientPool->getClient()->getWorkerService().destroy(id);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

void IWorker::setName(const std::string &name) {
    try {
        Ignis::clientPool->getClient()->getWorkerService().setName(id, name);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IWorker::parallelizeAbs(int64_t data_id, int64_t partitions) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getWorkerService().parallelize(_return, id, data_id, partitions);
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IWorker::parallelizeAbs(int64_t data_id, int64_t partitions, const ISource &src) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getWorkerService().parallelize4(_return, id, data_id, partitions, src.rpc());
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IWorker::importDataFrameAbs(IDataFrameId data) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getWorkerService().importDataFrame(_return, id, data);
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IWorker::importDataFrameAbs(IDataFrameId data, const ISource &src) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getWorkerService().importDataFrame3(_return, id, data, src.rpc());
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrame<std::string> IWorker::textFile(const std::string &path) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getWorkerService().textFile(_return, id, path);
        return IDataFrame<std::string>(_return);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrame<std::string> IWorker::textFile(const std::string &path, int64_t minPartitions) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getWorkerService().textFile3(_return, id, path, minPartitions);
        return IDataFrame<std::string>(_return);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IWorker::partitionObjectFileAbs(const std::string &path) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getWorkerService().partitionObjectFile(_return, id, path);
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IWorker::partitionObjectFileAbs(const std::string &path, const ISource &src) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getWorkerService().partitionObjectFile3(_return, id, path, src.rpc());
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrame<std::string> IWorker::partitionTextFile(const std::string &path) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getWorkerService().partitionTextFile(_return, id, path);
        return IDataFrame<std::string>(_return);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IWorker::partitionJsonFileAbs(const std::string &path, bool objectMapping) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getWorkerService().partitionJsonFile3a(_return, id, path, objectMapping);
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IWorker::partitionJsonFileAbs(const std::string &path, const ISource &src) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getWorkerService().partitionJsonFile3b(_return, id, path, src.rpc());
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

void IWorker::loadLibrary(const std::string &path) {
    try {
        Ignis::clientPool->getClient()->getWorkerService().loadLibrary(id, path);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

void IWorker::execute(const ISource &src) {
    try {
        Ignis::clientPool->getClient()->getWorkerService().execute(id, src.rpc());
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IWorker::executeToAbs(const ISource &src) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getWorkerService().executeTo(_return, id, src.rpc());
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

void IWorker::voidCall(const ISource &src) {
    try {
        Ignis::clientPool->getClient()->getWorkerService().voidCall(id, src.rpc());
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

void IWorker::voidCallAbs(const rpc::driver::IDataFrameId &df, const ISource &src) {
    try {
        Ignis::clientPool->getClient()->getWorkerService().voidCall3(id, df, src.rpc());
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IWorker::callAbs(const ISource &src) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getWorkerService().call(_return, id, src.rpc());
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}

IDataFrameId IWorker::callAbs(const rpc::driver::IDataFrameId &df, const ISource &src) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getWorkerService().call3(_return, id, df, src.rpc());
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex._cause); }
}
