
#ifndef IGNIS_IFUNCTION2_H
#define IGNIS_IFUNCTION2_H

#include "../IRegister.h"
#include "../../../data/IManager.h"
#include "../IContext.h"

namespace ignis {
    namespace executor {
        namespace api {
            namespace function {
                template<typename T1, typename T2, typename R>
                class IFunction2 {
                public:
                    const data::IManager<T1> type_t1;
                    const data::IManager<T2> type_t2;
                    const data::IManager<R> type_r;

                    virtual void before(IContext &context) {}

                    virtual R call(T1 &t1, T2 &t2, IContext &context) {}

                    virtual void after(IContext &context) {}
                };
            }
        }
    }
}

#endif //IGNIS_IFUNCTION2_H
