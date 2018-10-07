
#ifndef IGNIS_ILOGICERROR_H
#define IGNIS_ILOGICERROR_H

#include "IException.h"

namespace ignis {
    namespace exceptions {
        class ILogicError : public IException{
        public:

            ILogicError(const std::string message) : IException(message) {setExceptionName(this);}

            ILogicError(const std::string message, const IException &cause) : IException(message, cause) {setExceptionName(this);}

            ILogicError(const char *message) : IException(message) {setExceptionName(this);}

            ILogicError(const char *message, const IException &cause) : IException(message, cause) {setExceptionName(this);}

            virtual ~ILogicError() {}
        };
    }
}

#endif
