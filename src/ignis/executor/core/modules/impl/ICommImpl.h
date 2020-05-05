
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

                        std::string createGroup();

                        void joinGroupMembers(const std::string &group, const int64_t id, const int64_t size);

                        void joinToGroup(const std::string &group, const std::string &id);

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
                        void driverGather(const std::string &id);

                        template<typename Tp>
                        void driverGather0(const std::string &id);

                        void driverScatterVoid(const std::string &id, int64_t partitions);

                        template<typename Tp>
                        void driverScatter(const std::string &id, int64_t partitions);

                        virtual ~ICommImpl();

                    private:
                        MPI::Intracomm addComm(MPI::Intracomm &group, MPI::Intercomm &comm, MPI::Intracomm &local,
                                               bool detroyGroup = true);

                        MPI::Intracomm joinGroups(MPI::Intracomm &groupLow, MPI::Intracomm &groupUp);

                        MPI::Intracomm& getGroup(const std::string &id);

                        std::map<std::string, MPI::Intracomm> groups;
                    };
                }
            }
        }
    }
}

#include "ICommImpl.tcc"

#endif
