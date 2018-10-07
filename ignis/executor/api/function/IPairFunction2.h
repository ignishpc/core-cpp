
#ifndef IGNIS_IPAIRFUNCTION2_H
#define IGNIS_IPAIRFUNCTION2_H

#include "IFunction2.h"
#include "../IWriteIterator.h"

namespace ignis {
    namespace executor {
        namespace api {
            namespace function {
                template<typename K, typename T1, typename T2, typename R>
                class IPairFunction2 : public IFunction2<T1, T2, R> {
                public:

                    const api::IPairManager<K, T1> type_pt1;
                    const api::IPairManager<K, T1> type_pt2;
                    const api::IPairManager<K, T1> type_pr;

                    void writeCall(std::pair<K, T1> &t1, std::pair<K, T2> &t2, IContext &context,
                                   IWriteIterator<std::pair<K, R>> &writer){
                        writer.write(std::pair<K, R>(t1.first, this->call(t1.second, t2.second, context)));
                    }
                };
            }
        }
    }
}

#endif
