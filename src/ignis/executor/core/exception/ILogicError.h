
#ifndef IGNIS_ILOGICERROR_H
#define IGNIS_ILOGICERROR_H

#include "IException.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace exception {
                class ILogicError : public IException {
                public:
                    using IException::IException;

                    virtual ~ILogicError() {}
                };
            }// namespace exception
        }    // namespace core
    }        // namespace executor
}// namespace ignis

#endif
