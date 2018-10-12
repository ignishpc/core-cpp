
#ifndef IGNIS_IPAIRFLATFUNCTION_H
#define IGNIS_IPAIRFLATFUNCTION_H

#include "IPairFlatFunctionBase.h"

namespace ignis {
    namespace executor {
        namespace api {
            namespace function {

                template<typename T, typename K, typename V>
                class IPairFlatFunction : public IPairFlatFunctionBase<T, K, V> {
                public:

                    virtual void before(IContext &context) {}

                    virtual Iterable<std::pair<K, V>> call(T &t, IContext &context) = 0;

                    virtual void after(IContext &context) {}

                };
            }
        }
    }
}


#endif
