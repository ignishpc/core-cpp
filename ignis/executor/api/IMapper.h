
#ifndef IGNIS_IMAP_H
#define IGNIS_IMAP_H

#include "ILoadClass.h"
#include "../../data/serialization/ITypeHandle.h"
#include "IContext.h"
#include "IReadIterator.h"
#include "IWriteIterator.h"

namespace ignis {
    namespace executor {
        namespace api {

            template<typename T, typename R>
            class IMapper : public ILoadClass<IMapper<T, R>> {
            public:
                const auto type_t = data::serialization::ITypeHandle<T>();
                const auto type_r = data::serialization::ITypeHandle<R>();

                virtual void before(IContext &context) {}

                virtual void after(IContext &context) {}

                virtual void map(IReadIterator<T> &in, IWriteIterator<R> &out, IContext &context) = 0;
            };
        }
    }
}


#endif
