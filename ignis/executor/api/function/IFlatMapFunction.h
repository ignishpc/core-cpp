
#ifndef IGNIS_IFLATMAPFUNCTION_H
#define IGNIS_IFLATMAPFUNCTION_H

#include "executor/core//selector/ISelector.h"
#include "executor/api/IContext.h"

namespace ignis {
    namespace executor {
        namespace api {
            namespace function {

                template<typename T, typename R>
                class IFlatMapFunction{
                public:
                    typedef IFlatMapFunction<T,R> _IFlatMapFunction_type;
                    typedef T _T_type;
                    typedef R _R_type;

                    virtual void before(IContext &context) {}

                    virtual R call(T &v, IContext &context) {}

                    virtual void after(IContext &context) {}

                };
            }
        }
    }
}

#endif

