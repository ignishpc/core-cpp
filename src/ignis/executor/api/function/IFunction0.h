
#ifndef IGNIS_IFUNCTION0_H
#define IGNIS_IFUNCTION0_H

#include "ignis/executor/api/IContext.h"
#include "ignis/executor/core//selector/ISelector.h"

namespace ignis {
    namespace executor {
        namespace api {
            namespace function {

                template<typename R>
                class IFunction0 {
                public:
                    typedef IFunction0<R> _IFunction0_type;
                    typedef R _R_type;

                    virtual void before(IContext &context) {}

                    virtual R call(IContext &context) {
                        throw executor::core::exception::ILogicError("call function must be implemented");
                    }

                    virtual void after(IContext &context) {}
                };
            }// namespace function
        }    // namespace api
    }        // namespace executor
}// namespace ignis

#endif
