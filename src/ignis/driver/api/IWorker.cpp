
#include "IWorker.h"
#include "IDriverException.h"
#include "ignis/rpc/driver/IDriverException_types.h"

using namespace ignis::driver::api;
using ignis::rpc::driver::IDataFrameId;

IWorker::IWorker(const std::shared_ptr<ICluster> &cluster, const std::string &type) : cluster(cluster) {
    try {
        Ignis::clientPool->getClient()->getWorkerService().newInstance(id, cluster->id, type);
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

IWorker::IWorker(const std::shared_ptr<ICluster> &cluster, const std::string &name, const std::string &type) :
        cluster(cluster) {
    try {
        Ignis::clientPool->getClient()->getWorkerService().newInstance3a(id, cluster->id, name, type);
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

IWorker::IWorker(const std::shared_ptr<ICluster> &cluster, const std::string &type, int cores) : cluster(cluster) {
    try {
        Ignis::clientPool->getClient()->getWorkerService().newInstance3b(id, cluster->id, type, cores);
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

IWorker::IWorker(const std::shared_ptr<ICluster> &cluster, const std::string &name, const std::string &type,
                 int cores) : cluster(cluster) {
    try {
        Ignis::clientPool->getClient()->getWorkerService().newInstance4(id, cluster->id, name, type, cores);
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

std::shared_ptr<ICluster> IWorker::getCluster() {
    return cluster;
}

void IWorker::setName(const std::string &name) {
    try {
        Ignis::clientPool->getClient()->getWorkerService().setName(id, name);
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

IDataFrameId IWorker::parallelizeAbs(int64_t data_id) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getWorkerService().parallelize(_return, id, data_id);
        return _return;
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

IDataFrameId IWorker::parallelizeAbs(int64_t data_id, const ISource &src) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getWorkerService().parallelize3(_return, id, data_id, src.rpc());
        return _return;
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

IDataFrameId IWorker::importDataFrameAbs(IDataFrameId data) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getWorkerService().importDataFrame(_return, id, data);
        return _return;
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

IDataFrameId
IWorker::importDataFrameAbs(IDataFrameId data, int64_t partitions) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getWorkerService().importDataFrame3a(_return, id, data, partitions);
        return _return;
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

IDataFrameId
IWorker::importDataFrameAbs(IDataFrameId data, const ISource &src) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getWorkerService().importDataFrame3b(_return, id, data, src.rpc());
        return _return;
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

IDataFrameId
IWorker::importDataFrameAbs(IDataFrameId data, int64_t partitions, const ISource &src) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getWorkerService().importDataFrame4(_return, id, data, partitions, src.rpc());
        return _return;
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

std::shared_ptr<IDataFrame<std::string>> IWorker::textFile(const std::string &path) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getWorkerService().textFile(_return, id, path);
        return IDataFrame<std::string>::make(_return);
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

std::shared_ptr<IDataFrame<std::string>> IWorker::textFile(const std::string &path, int64_t minPartitions) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getWorkerService().textFile3(_return, id, path, minPartitions);
        return IDataFrame<std::string>::make(_return);
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

IDataFrameId IWorker::partitionObjectFileAbs(const std::string &path) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getWorkerService().partitionObjectFile(_return, id, path);
        return _return;
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

IDataFrameId IWorker::partitionObjectFileAbs(const std::string &path, const ISource &src) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getWorkerService().partitionObjectFile3(_return, id, path, src.rpc());
        return _return;
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

std::shared_ptr<IDataFrame<std::string>> IWorker::partitionTextFile(const std::string &path) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getWorkerService().partitionTextFile(_return, id, path);
        return IDataFrame<std::string>::make(_return);
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

IDataFrameId IWorker::partitionJsonFileAbs(const std::string &path) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getWorkerService().partitionJsonFile(_return, id, path);
        return _return;
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}

IDataFrameId IWorker::partitionJsonFileAbs(const std::string &path, const ISource &src) {
    try {
        IDataFrameId _return;
        Ignis::clientPool->getClient()->getWorkerService().partitionJsonFile(_return, id, path);
        return _return;
    } catch (rpc::driver::IDriverException &ex) {
        throw IDriverException(ex.message, ex._cause);
    }
}
