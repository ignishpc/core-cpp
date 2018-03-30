
#ifndef EXECUTORCPP_IOBJECT_H
#define EXECUTORCPP_IOBJECT_H

#include "../../../data/ITypeInfo.h"
#include "../../api/IReadIterator.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {
                class IObject {
                public:

                    IObject(data::ITypeInfo &type) : type(type) {}

                    template<typename T>
                    api::IReadIterator<T> iterator();

                private:
                    data::ITypeInfo type;
                };
            }
        }
    }
}

#endif
