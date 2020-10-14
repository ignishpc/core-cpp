
#ifndef IGNIS_IFUNCTION_H
#define IGNIS_IFUNCTION_H

#include "ignis/executor/api/IContext.h"
#include "ignis/executor/core/selector/ISelector.h"

namespace ignis {
    namespace executor {
        namespace api {
            namespace function {

                template<typename T, typename R>
                class IFunction {
                public:
                    typedef IFunction<T, R> _IFunction_type;
                    typedef T _T_type;
                    typedef R _R_type;

                    virtual void before(IContext &context) {}

                    virtual R call(T &v, IContext &context) {
                        throw executor::core::exception::ILogicError("call function must be implemented");
                    }

                    virtual void after(IContext &context) {}
                };
            }// namespace function
        }    // namespace api
    }        // namespace executor
}// namespace ignis

#endif
