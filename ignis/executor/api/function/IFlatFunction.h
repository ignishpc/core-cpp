
#ifndef IGNIS_IFLATFUNCTION_H
#define IGNIS_IFLATFUNCTION_H

#include "IFlatFunctionBase.h"

namespace ignis {
    namespace executor {
        namespace api {
            namespace function {

                template<typename T, typename R>
                class IFlatFunction : public IFlatFunctionBase<T, R> {
                public:

                    virtual void before(IContext &context) {}

                    virtual Iterable<R> call(T &t, IContext &context) {}

                    virtual void after(IContext &context) {}

                };
            }
        }
    }
}

#endif
