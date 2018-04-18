
#ifndef IGNIS_IMAPPER_H
#define IGNIS_IMAPPER_H

#include "IRegister.h"
#include "../../data/IManager.h"
#include "IContext.h"
#include "IReadIterator.h"
#include "IWriteIterator.h"

namespace ignis {
    namespace executor {
        namespace api {

            template<typename T, typename R>
            class IMapper{
            public:
                const data::IManager<T> type_t;
                const data::IManager<R> type_r;

                virtual void before(IContext &context) {}

                virtual void after(IContext &context) {}

                virtual void map(IReadIterator<T> &in, IWriteIterator<R> &out, IContext &context) = 0;
            };
        }
    }
}

#endif
