
#ifndef IGNIS_IREDUCER_H
#define IGNIS_IREDUCER_H

#include <utility>
#include "IRegister.h"
#include "../../data/IManager.h"
#include "IContext.h"
#include "IReadIterator.h"
#include "IWriteIterator.h"

namespace ignis {
    namespace executor {
        namespace api {

            template<typename K, typename V, typename R>
            class IReducer {
            public:
                const data::IManager<std::pair<K,V>> type_t;
                const data::IManager<R> type_r;

                virtual void before(IContext &context) {}

                virtual void after(IContext &context) {}

                virtual void reduce(IReadIterator<std::pair<K,V>> &in, IWriteIterator<R> &out, IContext &context) = 0;
            };
        }
    }
}


#endif
