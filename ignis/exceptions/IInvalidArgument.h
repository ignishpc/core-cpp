
#ifndef IGNIS_IINVALIDARGUMENT_H
#define IGNIS_IINVALIDARGUMENT_H

#include "IException.h"

namespace ignis {
    namespace exceptions {
        class IInvalidArgument : public IException {
        public:
            IInvalidArgument(const std::string &message) : IException(message) {setExceptionName(this);}

            IInvalidArgument(const std::string &message, const IException &cause) : IException(message, cause) {setExceptionName(this);}

            IInvalidArgument(const char *message) : IException(message) {setExceptionName(this);}

            IInvalidArgument(const char *message, const IException &cause) : IException(message, cause) {setExceptionName(this);}
        };
    }
}

#endif
