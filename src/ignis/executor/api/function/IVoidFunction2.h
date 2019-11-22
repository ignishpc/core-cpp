
#ifndef IGNIS_IVOIDFUNCTION2_H
#define IGNIS_IVOIDFUNCTION2_H

#include "../../core/selector/ISelector.h"
#include "../IContext.h"

namespace ignis {
    namespace executor {
        namespace api {
            namespace function {

                template<typename T1, typename T2, typename R>
                class IVoidFunction2 {
                public:
                    typedef IVoidFunction2<T1, T2, R> _IVoidFunction2_type;
                    typedef T _T1_type;
                    typedef T _T2_type;
                    typedef R _R_type;

                    virtual void before(IContext &context) {}

                    virtual R call(T1 &v1, T2 &v2, IContext &context) {}

                    virtual void after(IContext &context) {}

                };
            }
        }
    }
}

#endif

