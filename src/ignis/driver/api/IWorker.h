
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
                IWorker(const ICluster &cluster, const std::string &type);

                IWorker(const ICluster &cluster, const std::string &name, const std::string &type);

                IWorker(const ICluster &cluster, const std::string &type, int cores, int instances);

                IWorker(const ICluster &cluster, const std::string &name, const std::string &type, int cores,
                        int instances);

                ICluster getCluster();

                void start();

                void destroy();

                void setName(const std::string &name);

                template<typename C>
                IDataFrame<typename C::value_type> parallelize(const C &data, int64_t partitions) {
                    auto data_id = Ignis::driverContext().parallelize<C>(data);
                    return IDataFrame<typename C::value_type>(parallelizeAbs(data_id, partitions));
                }

                template<typename C>
                IDataFrame<typename C::value_type> parallelize(const C &data, int64_t partitions, const ISource &src) {
                    auto data_id = Ignis::driverContext().parallelize<C>(data);
                    return IDataFrame<typename C::value_type>(parallelizeAbs(data_id, partitions, src));
                }

                template<typename Tp>
                IDataFrame<Tp> importDataFrame(const IDataFrame<Tp> &data) {
                    return IDataFrame<Tp>(importDataFrameAbs(data.id));
                }

                template<typename Tp>
                IDataFrame<Tp> importDataFrame(const IDataFrame<Tp> &data, const ISource &src) {
                    return IDataFrame<Tp>(importDataFrameAbs(data.id, src));
                }

                IDataFrame<std::string> plainFile(const std::string &path, char delim);

                IDataFrame<std::string> plainFile(const std::string &path, int64_t minPartitions, char delim);

                IDataFrame<std::string> textFile(const std::string &path);

                IDataFrame<std::string> textFile(const std::string &path, int64_t minPartitions);

                template<typename Tp>
                IDataFrame<Tp> partitionObjectFile(const std::string &path) {
                    return IDataFrame<Tp>(partitionObjectFileAbs(path));
                }

                template<typename Tp>
                IDataFrame<Tp> partitionObjectFile(const std::string &path, const ISource &src) {
                    return IDataFrame<Tp>(partitionObjectFileAbs(path, src));
                }

                IDataFrame<std::string> partitionTextFile(const std::string &path);

                template<typename Tp>
                IDataFrame<Tp> partitionJsonFile(const std::string &path, bool objectMapping = false) {
                    return IDataFrame<Tp>(partitionJsonFileAbs(path, objectMapping));
                }

                template<typename Tp>
                IDataFrame<Tp> partitionJsonFile(const std::string &path, const ISource &src) {
                    return IDataFrame<Tp>(partitionJsonFileAbs(path, src));
                }

                void loadLibrary(const std::string &path);

                void execute(const ISource &src);

                template<typename Tp>
                IDataFrame<Tp> executeTo(const ISource &src) {
                    return IDataFrame<Tp>(executeToAbs(src));
                }

                void voidCall(const ISource &src);

                template<typename Tp>
                void voidCall(const IDataFrame<Tp> &df, const ISource &src) {
                    voidCallAbs(df.id, src);
                }

                template<typename R>
                IDataFrame<R> call(const ISource &src) {
                    return IDataFrame<R>(callAbs(src));
                }

                template<typename Tp, typename R>
                IDataFrame<R> call(const IDataFrame<Tp> &df, const ISource &src) {
                    return IDataFrame<R>(callAbs(df.id, src));
                }

            private:
                rpc::driver::IWorkerId id;
                ICluster cluster;

                rpc::driver::IDataFrameId parallelizeAbs(int64_t data_id, int64_t partitions);

                rpc::driver::IDataFrameId parallelizeAbs(int64_t data_id, int64_t partitions, const ISource &src);

                rpc::driver::IDataFrameId importDataFrameAbs(rpc::driver::IDataFrameId data);

                rpc::driver::IDataFrameId importDataFrameAbs(rpc::driver::IDataFrameId data, const ISource &src);

                rpc::driver::IDataFrameId partitionObjectFileAbs(const std::string &path);

                rpc::driver::IDataFrameId partitionObjectFileAbs(const std::string &path, const ISource &src);

                rpc::driver::IDataFrameId partitionJsonFileAbs(const std::string &path, bool objectMapping);

                rpc::driver::IDataFrameId partitionJsonFileAbs(const std::string &path, const ISource &src);

                rpc::driver::IDataFrameId executeToAbs(const ISource &src);

                void voidCallAbs(const rpc::driver::IDataFrameId &df, const ISource &src);

                rpc::driver::IDataFrameId callAbs(const ISource &src);

                rpc::driver::IDataFrameId callAbs(const rpc::driver::IDataFrameId &df, const ISource &src);
            };
        }// namespace api
    }    // namespace driver
}// namespace ignis

#endif
