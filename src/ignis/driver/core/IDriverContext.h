
#ifndef IGNIS_IDRIVERCONTEXT_H
#define IGNIS_IDRIVERCONTEXT_H

#include <memory>
#include <vector>
#include "ignis/rpc/ISource_types.h"

namespace ignis {
    namespace driver {
        namespace core {
            class IDriverContext {
            public:

                template<typename Tp>
                rpc::ISource registerType();

                template<typename C>
                int64_t parallelize(const C &collection);

                template<typename Tp>
                std::vector<Tp> collect(int64_t id);

                template<typename Tp>
                Tp collect1(int64_t id);

            };
        }
    }
}

#include "IDriverPartition.tcc"

#endif
