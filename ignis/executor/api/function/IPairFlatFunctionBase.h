
#ifndef IGNIS_IPAIRFLATFUNCTIONBASE_H
#define IGNIS_IPAIRFLATFUNCTIONBASE_H

#include "../IRegister.h"
#include "../IManager.h"
#include "../IContext.h"
#include "../IWriteIterator.h"
#include "../Iterable.h"

namespace ignis {
    namespace executor {
        namespace api {
            namespace function {

                template<typename T, typename K, typename V>
                class IPairFlatFunctionBase {
                public:
                    const IManager<T> type_t;
                    const IPairManager<K, V> pair_kv;

                    virtual Iterable<std::pair<K, V>> call(T &t, IContext &context) = 0;

                    virtual void write(T &t, IContext &context, IWriteIterator<typename IPairManager<K, V>::Class> &writer) {
                        Iterable<std::pair<K, V>> it = call(t, context).readIterator();
                        while(it.hasNext()){
                            writer.write(it.next());
                        }
                    }
                };
            }
        }
    }
}

#endif