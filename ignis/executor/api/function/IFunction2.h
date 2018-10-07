
#ifndef IGNIS_IFUNCTION2_H
#define IGNIS_IFUNCTION2_H

#include "../IRegister.h"
#include "../IManager.h"
#include "../IContext.h"

namespace ignis {
    namespace executor {
        namespace api {
            namespace function {
                template<typename T1, typename T2, typename R>
                class IFunction2 {
                public:
                    const api::IManager <T1> type_t1;
                    const api::IManager <T2> type_t2;
                    const api::IManager <R> type_r;

                    virtual void writeCall(T1 &t1, T2 &t2, IContext &context, IWriteIterator<R> &writer) {
                        writer.write(call(t1, t2, context));
                    }

                    virtual void before(IContext &context) {}

                    virtual R call(T1 &t1, T2 &t2, IContext &context) {}

                    virtual void after(IContext &context) {}
                };
            }
        }
    }
}

#endif
