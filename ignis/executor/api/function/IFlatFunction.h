
#ifndef IGNIS_IFLATMAPFUNCTION_H
#define IGNIS_IFLATMAPFUNCTION_H

#include "IFunction.h"
#include "../Iterable.h"

namespace ignis {
    namespace executor {
        namespace api {
            namespace function {
                template<typename T, typename R>
                class IFlatFunction : public IFunction<T, Iterable<R>> {
                };
            }
        }
    }
}

#endif
