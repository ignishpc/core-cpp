
#ifndef IGNIS_IELEMENTS_H
#define IGNIS_IELEMENTS_H

#include "executor/api/IVector.h"

namespace ignis{
    namespace executor{
        namespace core{
            template<typename Tp>
            struct IElements {
                static api::IVector<Tp> create(int n, int seed);
            };
        }
    }
}

#include "IElements.tcc"
#endif
