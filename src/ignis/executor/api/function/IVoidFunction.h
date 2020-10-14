
#ifndef IGNIS_IVOIDFUNCTION_H
#define IGNIS_IVOIDFUNCTION_H

#include "../../core/selector/ISelector.h"
#include "../IContext.h"

namespace ignis {
    namespace executor {
        namespace api {
            namespace function {

                template<typename T>
                class IVoidFunction {
                public:
                    typedef IVoidFunction<T> _IVoidFunction_type;
                    typedef T _T_type;

                    virtual void before(IContext &context) {}

                    virtual void call(T &v, IContext &context) {
                        throw executor::core::exception::ILogicError("call function must be implemented");
                    }

                    virtual void after(IContext &context) {}
                };
            }// namespace function
        }    // namespace api
    }        // namespace executor
}// namespace ignis

#endif
