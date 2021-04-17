
#ifndef IGNIS_IBEFOREFUNCTION_H
#define IGNIS_IBEFOREFUNCTION_H

#include "ignis/executor/api/IContext.h"
#include "ignis/executor/core/selector/ISelector.h"

namespace ignis {
    namespace executor {
        namespace api {
            namespace function {

                template<typename T>
                class IBeforeFunction {
                public:
                    typedef IBeforeFunction<R> _IBeforeFunction_type;
                    typedef T _T_type;

                    virtual void before(IContext &context) {}

                };
            }// namespace function
        }    // namespace api
    }        // namespace executor
}// namespace ignis

#endif
