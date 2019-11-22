
#ifndef IGNIS_IINVALIDARGUMENT_H
#define IGNIS_IINVALIDARGUMENT_H

#include "IException.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace exception {
                class IInvalidArgument : public IException {
                public:
                    using IException::IException;

                    virtual ~IInvalidArgument() {}
                };
            }
        }
    }
}

#endif

