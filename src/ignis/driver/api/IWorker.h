
#ifndef IGNIS_IWORKER_H
#define IGNIS_IWORKER_H

#include "ICluster.h"
#include "IDataFrame.h"
#include "ISource.h"
#include "ignis/rpc/driver/IWorkerService_types.h"

namespace ignis {
    namespace driver {
        namespace api {
            class IWorker {
            public:

                IWorker(const std::shared_ptr<ICluster> &cluster, const std::string &type);

                IWorker(const std::shared_ptr<ICluster> &cluster, const std::string &name, const std::string &type);

                IWorker(const std::shared_ptr<ICluster> &cluster, const std::string &type, int cores);

                IWorker(const std::shared_ptr<ICluster> &cluster, const std::string &name, const std::string &type,
                        int cores);

                std::shared_ptr<ICluster> getCluster();

                void setName(const std::string &name);

                template<typename C>
                std::shared_ptr<IDataFrame<typename C::value_type>> parallelize(const C &data, int64_t partitions) {
                    auto data_id = Ignis::driverContext().parallelize<C>(data, partitions);
                    return IDataFrame<typename C::value_type>::make(parallelizeAbs(data_id));
                }

                template<typename C>
                std::shared_ptr<IDataFrame<typename C::value_type>>
                parallelize(const C &data, int64_t partitions, const ISource &src) {
                    auto data_id = Ignis::driverContext().parallelize<C>(data, partitions);
                    return IDataFrame<typename C::value_type>::make(parallelizeAbs(data_id, src));
                }

                template<typename Tp>
                std::shared_ptr<IDataFrame<Tp>> importDataFrame(const std::shared_ptr<IDataFrame<Tp>> &data) {
                    return IDataFrame<Tp>::make(importDataFrameAbs(data.id));
                }

                template<typename Tp>
                std::shared_ptr<IDataFrame<Tp>>
                importDataFrame(const std::shared_ptr<IDataFrame<Tp>> &data, int64_t partitions) {
                    return IDataFrame<Tp>::make(importDataFrameAbs(data.id, partitions));
                }

                template<typename Tp>
                std::shared_ptr<IDataFrame<Tp>>
                importDataFrame(const std::shared_ptr<IDataFrame<Tp>> &data, const ISource &src) {
                    return IDataFrame<Tp>::make(importDataFrameAbs(data.id, src));
                }

                template<typename Tp>
                std::shared_ptr<IDataFrame<Tp>>
                importDataFrame(const std::shared_ptr<IDataFrame<Tp>> &data, int64_t partitions, const ISource &src) {
                    return IDataFrame<Tp>::make(importDataFrameAbs(data.id, partitions, src));
                }

                std::shared_ptr<IDataFrame<std::string>> textFile(const std::string &path);

                std::shared_ptr<IDataFrame<std::string>> textFile(const std::string &path, int64_t minPartitions);

                template<typename Tp>
                std::shared_ptr<IDataFrame<Tp>> partitionObjectFile(const std::string &path) {
                    return IDataFrame<Tp>::make(partitionObjectFileAbs(path));
                }

                template<typename Tp>
                std::shared_ptr<IDataFrame<Tp>> partitionObjectFile(const std::string &path, const ISource &src) {
                    return IDataFrame<Tp>::make(partitionObjectFileAbs(path, src));
                }

                std::shared_ptr<IDataFrame<std::string>> partitionTextFile(const std::string &path);

                template<typename Tp>
                std::shared_ptr<IDataFrame<Tp>> partitionJsonFile(const std::string &path, bool objectMapping=false) {
                    return IDataFrame<Tp>::make(partitionJsonFileAbs(path, objectMapping));
                }

                template<typename Tp>
                std::shared_ptr<IDataFrame<Tp>> partitionJsonFile(const std::string &path, const ISource &src) {
                    return IDataFrame<Tp>::make(partitionJsonFileAbs(path, src));
                }


            private:
                rpc::driver::IWorkerId id;
                std::shared_ptr<ICluster> cluster;

                rpc::driver::IDataFrameId parallelizeAbs(int64_t data_id);

                rpc::driver::IDataFrameId parallelizeAbs(int64_t data_id, const ISource &src);

                rpc::driver::IDataFrameId importDataFrameAbs(rpc::driver::IDataFrameId data);

                rpc::driver::IDataFrameId importDataFrameAbs(rpc::driver::IDataFrameId data, int64_t partitions);

                rpc::driver::IDataFrameId importDataFrameAbs(rpc::driver::IDataFrameId data, const ISource &src);

                rpc::driver::IDataFrameId
                importDataFrameAbs(rpc::driver::IDataFrameId data, int64_t partitions, const ISource &src);

                rpc::driver::IDataFrameId partitionObjectFileAbs(const std::string &path);

                rpc::driver::IDataFrameId partitionObjectFileAbs(const std::string &path, const ISource &src);

                rpc::driver::IDataFrameId partitionJsonFileAbs(const std::string &path, bool objectMapping);

                rpc::driver::IDataFrameId partitionJsonFileAbs(const std::string &path, const ISource &src);
            };
        }
    }
}

#endif
