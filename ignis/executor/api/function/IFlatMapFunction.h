
#ifndef IGNIS_IFLATMAPFUNCTION_H
#define IGNIS_IFLATMAPFUNCTION_H

#include "IFunction.h"
#include "../Iterable.h"

namespace ignis {
    namespace executor {
        namespace api {
            namespace function {
                template<typename T, typename R>
                class IFlatMapFunction : IFunction<T, Iterable<R>> {
                public:
                    virtual void before(IContext &context) override {}

                    virtual Iterable<R> call(T &t, IContext &context) override {}

                    virtual void after(IContext &context) override {}
                };
            }
        }
    }
}

#endif
