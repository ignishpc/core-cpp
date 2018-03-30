
#ifndef IGNIS_IMAP_H
#define IGNIS_IMAP_H

#include <utility>
#include "ILoadClass.h"
#include "../../data/ITypeInfo.h"
#include "IContext.h"
#include "IReadIterator.h"
#include "IWriteIterator.h"

namespace ignis {
    namespace executor {
        namespace api {

            template<typename K, typename V, typename R, typename Info = data::ITypeInfo>
            class IReducer : public ILoadClass<IReducer<K, V, R>> {
            public:
                const data::ITypeInfo type_t = Info::getInfo<std::pair<K,V>>();
                const data::ITypeInfo type_r = Info::getInfo<R>();

                virtual void before(IContext &context) {}

                virtual void after(IContext &context) {}

                virtual void reduce(IReadIterator<std::pair<K,V>> &in, IWriteIterator<R> &out, IContext &context) = 0;
            };
        }
    }
}


#endif
