
#ifndef IGNIS_IFUNCTION_H
#define IGNIS_IFUNCTION_H

#include "../IRegister.h"
#include "../../../data/IManager.h"
#include "../IContext.h"

namespace ignis {
    namespace executor {
        namespace api {
            namespace function {
                template<typename T, typename R>
                class IFunction {
                public:
                    const data::IManager<T> type_t;
                    const data::IManager<R> type_r;

                    virtual void before(IContext &context) {}

                    virtual R call(T &t, IContext &context) {}

                    virtual void after(IContext &context) {}
                };
            }
        }
    }
}

#endif
