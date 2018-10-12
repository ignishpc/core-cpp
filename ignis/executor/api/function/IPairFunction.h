
#ifndef IGNIS_IPAIRFUNCTION_H
#define IGNIS_IPAIRFUNCTION_H

#include "IPairFunctionBase.h"

namespace ignis {
    namespace executor {
        namespace api {
            namespace function {

                template<typename T, typename K, typename V>
                class IPairFunction : public IPairFunctionBase{
                public:
                    virtual void before(IContext &context) {}

                    virtual std::pair<K, V> call(T &t, IContext &context) {};

                    virtual void after(IContext &context) {}

                };
            }
        }
    }
}

#endif
