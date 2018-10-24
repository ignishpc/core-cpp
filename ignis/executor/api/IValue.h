
#ifndef IGNIS_IVALUE_H
#define IGNIS_IVALUE_H

#include "IValueBase.h"
#include "IContext.h"

namespace ignis {
    namespace executor {
        namespace api {

            template<typename T>
            class IValue : public IValueBase<T> {

                T *get(IContext &context) { return NULL; }

            };

        }
    }
}

#endif
