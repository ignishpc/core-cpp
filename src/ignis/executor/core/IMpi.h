
#ifndef IGNIS_IMPI_H
#define IGNIS_IMPI_H

#include <mpi.h>
#include "storage/IPartition.h"
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

                void barrier();

                bool isRoot(int root);

                int rank();

                int executors();

                template<typename Tp>
                bool isPrimitiveType();

            private:

                std::vector<int> szVector(const std::vector<std::pair<int,int>> &elems_szv);

                int sumElems(const std::vector<std::pair<int,int>> &elems_szv);

                std::vector<int> displs(const std::vector<int> &sz);

                void move(void *begin, size_t n, size_t displ);

                template<typename Tp>
                struct isPrimitive;

                IPropertyParser &properties;
                const MPI::Intracomm &comm;
            };
        }
    }
}

#include "IMpi.tcc"

#endif

