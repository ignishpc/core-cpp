
#ifndef IGNIS_IVOIDFUNCTION2_H
#define IGNIS_IVOIDFUNCTION2_H

#include "ignis/executor/api/IContext.h"
#include "ignis/executor/core/selector/ISelector.h"

namespace ignis {
    namespace executor {
        namespace api {
            namespace function {

                template<typename T1, typename T2>
                class IVoidFunction2 {
                public:
                    typedef IVoidFunction2<T1, T2> _IVoidFunction2_type;
                    typedef T1 _T1_type;
                    typedef T2 _T2_type;

                    virtual void before(IContext &context) {}

                    virtual void call(T1 &v1, T2 &v2, IContext &context) {
                        throw executor::core::exception::ILogicError("call function must be implemented");
                    }

                    virtual void after(IContext &context) {}
                };
            }// namespace function
        }    // namespace api
    }        // namespace executor
}// namespace ignis

#endif
