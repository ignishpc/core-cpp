
#ifndef IGNIS_IPAIRFUNCTIONBASE_H
#define IGNIS_IPAIRFUNCTIONBASE_H

#include "../IRegister.h"
#include "../IManager.h"
#include "../IContext.h"
#include "../IWriteIterator.h"

namespace ignis {
    namespace executor {
        namespace api {
            namespace function {

                template<typename T, typename K, typename V>
                class IPairFunctionBase {
                public:
                    const IManager<T> type_t;
                    const IPairManager<K, V> pair_kv;

                    virtual std::pair<K, V> call(T &t, IContext &context) = 0;

                    virtual void write(T &t, IContext &context, IWriteIterator<typename IPairManager<K, V>::Class> &writer) {
                        writer.write(call(t, context));
                    }

                };
            }
        }
    }
}

#endif
