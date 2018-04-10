
#ifndef IGNIS_IMAP_H
#define IGNIS_IMAP_H

#include <utility>
#include "ILoadClass.h"
#include "../../data/serialization/ITypeHandle.h"
#include "IContext.h"
#include "IReadIterator.h"
#include "IWriteIterator.h"

namespace ignis {
    namespace executor {
        namespace api {

            template<typename K, typename V, typename R>
            class IReducer : public ILoadClass<IReducer<K, V, R>> {
            public:
                const auto type_t = data::serialization::ITypeHandle<K>();
                const auto type_r = data::serialization::ITypeHandle<V>();

                virtual void before(IContext &context) {}

                virtual void after(IContext &context) {}

                virtual void reduce(IReadIterator<std::pair<K,V>> &in, IWriteIterator<R> &out, IContext &context) = 0;
            };
        }
    }
}


#endif
