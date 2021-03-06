
#ifndef IGNIS_IMPI_H
#define IGNIS_IMPI_H

#include "IPartitionTools.h"
#include "IPropertyParser.h"
#include "ignis/executor/api/IContext.h"
#include "ignis/executor/core/ILog.h"
#include "storage/IPartition.h"
#include "storage/IVoidPartition.h"
#include <mpi.h>

namespace ignis {
    namespace executor {
        namespace core {
            class IMpi {
            public:
                IMpi(IPropertyParser &properties, IPartitionTools &partition_tools, api::IContext &context);

                template<typename Tp>
                void gather(storage::IPartition<Tp> &part, int root);

                template<typename Tp>
                void bcast(storage::IPartition<Tp> &part, int root);

                template<typename Tp>
                void driverGather(const MPI::Intracomm &group, storage::IPartitionGroup<Tp> &part_group);

                template<typename Tp>
                void driverGather0(const MPI::Intracomm &group, storage::IPartitionGroup<Tp> &part_group);

                template<typename Tp>
                void driverScatter(const MPI::Intracomm &group, storage::IPartitionGroup<Tp> &part_group,
                                   int64_t partitions);

                void driverScatterVoid(const MPI::Intracomm &group,
                                       storage::IPartitionGroup<storage::IVoidPartition::VOID_TYPE> &part_group,
                                       int64_t partitions);

                typedef struct {
                    bool same_protocol, same_storage;
                } MsgOpt;

                MsgOpt getMsgOpt(const MPI::Intracomm &group, const std::string &ptype, bool send, int other, int tag);

                template<typename Tp>
                void send(const MPI::Intracomm &group, storage::IPartition<Tp> &part, int dest, int tag);

                template<typename Tp>
                void send(const MPI::Intracomm &group, storage::IPartition<Tp> &part, int dest, int tag,
                          const MsgOpt &o);

                template<typename Tp>
                void send(storage::IPartition<Tp> &part, int dest, int tag);

                template<typename Tp>
                void recv(const MPI::Intracomm &group, storage::IPartition<Tp> &part, int source, int tag);

                template<typename Tp>
                void recv(const MPI::Intracomm &group, storage::IPartition<Tp> &part, int source, int tag,
                          const MsgOpt &o);

                void recvVoid(const MPI::Intracomm &group, storage::IVoidPartition &part, int source, int tag);

                void recvVoid(const MPI::Intracomm &group, storage::IVoidPartition &part, int source, int tag,
                              const MsgOpt &o);

                template<typename Tp>
                void recv(storage::IPartition<Tp> &part, int source, int tag);

                template<typename Tp>
                void sendRcv(storage::IPartition<Tp> &send, storage::IPartition<Tp> &rcv, int other, int tag);

                void barrier();

                bool isRoot(int root);

                int rank();

                int executors();

                const MPI::Intracomm &native();

                template<typename Tp>
                bool isContiguousType();

            private:
                std::vector<int> szVector(const std::vector<std::pair<int, int>> &elems_szv);

                int sumElems(const std::vector<std::pair<int, int>> &elems_szv);

                std::vector<int> displs(const std::vector<int> &sz);

                void move(void *begin, size_t n, size_t displ);

                template<typename Tp>
                void gatherImpl(const MPI::Intracomm &group, storage::IPartition<Tp> &part, int root,
                                bool same_protocol);

                template<typename Tp>
                void sendRecv(storage::IPartition<Tp> &part, int source, int dest, int tag);

                template<typename Tp>
                void sendRecv(const MPI::Intracomm &group, storage::IPartition<Tp> &part, int source, int dest,
                              int tag, const MsgOpt &o);

                template<typename Tp>
                void sendRecvImpl(const MPI::Intracomm &group, storage::IPartition<Tp> &part, int source, int dest,
                                  int tag, bool same_protocol);

                IPropertyParser &properties;
                IPartitionTools &partition_tools;
                api::IContext &context;
            };
        }// namespace core
    }    // namespace executor
}// namespace ignis

#include "IMpi.tcc"

#endif
