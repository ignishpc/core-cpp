
#ifndef UNTITLED_ICONTAINER_H
#define UNTITLED_ICONTAINER_H

#include <memory>
#include "IHandle.h"
#include "IType.h"

namespace ignis {
    namespace data {
        namespace serialization {
            template<typename T=bool, template<typename, typename> typename C=IHandle>
            class IContainer {
            public:

                virtual std::shared_ptr<IHandle<>> new_instance(IType<> *sc) {
                    if (sc != NULL) {
                        return sc->instance((T *) NULL);
                    }
                    return std::shared_ptr<IHandle<>>((IHandle<> *) new C<T, bool>());
                }
            };
        }
    }
}

#endif
