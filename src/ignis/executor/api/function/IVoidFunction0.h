
#ifndef IGNIS_IVOIDFUNCTION0_H
#define IGNIS_IVOIDFUNCTION0_H

#include "ignis/executor/core/selector/ISelector.h"
#include "ignis/executor/api/IContext.h"

namespace ignis {
    namespace executor {
        namespace api {
            namespace function {
                template<typename T>
                class IVoidFunction0 {
                public:
                    typedef IVoidFunction0<T> _IVoidFunction0_type;
                    typedef T _T_type;

                    virtual void before(IContext &context) {}

                    virtual void call(IContext &context) {}

                    virtual void after(IContext &context) {}

                };
            }
        }
    }
}

#endif

