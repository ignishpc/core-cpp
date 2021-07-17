
#ifndef IGNIS_ICOMMIMPL_H
#define IGNIS_ICOMMIMPL_H

#include "IBaseImpl.h"
#include <map>
#include <mpi.h>

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                namespace impl {
                    class ICommImpl : public IBaseImpl {
                    public:
                        ICommImpl(std::shared_ptr<IExecutorData> &executorData);

                        std::string openGroup();

                        void closeGroup();

                        void joinToGroup(const std::string &id, bool leader);

                        void joinToGroupName(const std::string &id, bool leader, const std::string &name);

                        bool hasGroup(const std::string &id);

                        void destroyGroup(const std::string &id);

                        void destroyGroups();

                        int8_t getProtocol();

                        template<typename Tp>
                        std::vector<std::string> getPartitions(const int8_t protocol, int64_t minPartitions);

                        void setPartitionsVoid(const std::vector<std::string> &partitions);

                        template<typename Tp>
                        void setPartitions(const std::vector<std::string> &partitions);

                        template<typename Tp>
                        void newEmptyPartitions(int64_t n);

                        void newEmptyPartitionsVoid(int64_t n);

                        template<typename Tp>
                        void driverGather(const std::string &group);

                        template<typename Tp>
                        void driverGather0(const std::string &group);

                        void driverScatterVoid(const std::string &group, int64_t partitions);

                        template<typename Tp>
                        void driverScatter(const std::string &group, int64_t partitions);

                        int32_t enableMultithreading(const std::string& group);

                        template<typename Tp>
                        void send(const std::string &group, int64_t partition, int64_t dest, int64_t thread);

                        template<typename Tp>
                        void recv(const std::string &group, int64_t partition, int64_t source, int64_t thread);

                        void recvVoid(const std::string &group, int64_t partition, int64_t source, int64_t thread);

                        virtual ~ICommImpl();

                    private:
                        MPI::Intracomm joinToGroupImpl(const std::string &id, bool leader);

                        MPI::Intracomm &getGroup(const std::string &id);

                        std::map<std::string, MPI::Intracomm> groups;
                    };
                }// namespace impl
            }    // namespace modules
        }        // namespace core
    }            // namespace executor
}// namespace ignis

#include "ICommImpl.tcc"

#endif
