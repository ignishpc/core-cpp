
#ifndef IGNIS_IMPI_H
#define IGNIS_IMPI_H

#include <mpi.h>
#include "storage/IPartition.h"
#include "storage/IVoidPartition.h"
#include "IPropertyParser.h"

namespace ignis {
    namespace executor {
        namespace core {
            class IMpi {
            public:
                IMpi(IPropertyParser &properties, const MPI::Intracomm &comm);

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

                template<typename Tp>
                void send(const MPI::Intracomm &group,storage::IPartition<Tp> &part, int dest, int tag);

                template<typename Tp>
                void send(storage::IPartition<Tp> &part, int dest, int tag);

                template<typename Tp>
                void recv(const MPI::Intracomm &group,storage::IPartition<Tp> &part, int source, int tag);

                template<typename Tp>
                void recv(storage::IPartition<Tp> &part, int source, int tag);

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
                void sendRecv(storage::IPartition<Tp> &part, int source, int dest, int tag);

                IPropertyParser &properties;
                const MPI::Intracomm &comm;
            };
        }
    }
}

#include "IMpi.tcc"

#endif

