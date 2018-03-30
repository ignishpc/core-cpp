
#ifndef IGNIS_IMAP_H
#define IGNIS_IMAP_H

#include "ILoadClass.h"
#include "../../data/ITypeInfo.h"
#include "IContext.h"
#include "IReadIterator.h"
#include "IWriteIterator.h"

namespace ignis {
    namespace executor {
        namespace api {

            template<typename T, typename R, typename Info = data::ITypeInfo>
            class IMapper : public ILoadClass<IMapper<T, R>> {
            public:
                const data::ITypeInfo type_t = Info::getInfo<T>();
                const data::ITypeInfo type_r = Info::getInfo<R>();

                virtual void before(IContext &context) {}

                virtual void after(IContext &context) {}

                virtual void map(IReadIterator<T> &in, IWriteIterator<R> &out, IContext &context) = 0;
            };
        }
    }
}


#endif
