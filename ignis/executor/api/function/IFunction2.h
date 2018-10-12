
#ifndef IGNIS_IFUNCTION2_H
#define IGNIS_IFUNCTION2_H

#include "IFunction2Base.h"

namespace ignis {
    namespace executor {
        namespace api {
            namespace function {
                
                template<typename T1, typename T2, typename R>
                class IFunction2 : public IFunction2Base<T1, T2, R> {
                public:

                    virtual void before(IContext &context) {}

                    virtual R call(T1 &t1, T2 &t2, IContext &context) {}

                    virtual void after(IContext &context) {}
                };
            }
        }
    }
}

#endif
