
#ifndef IGNIS_IPRINTER_H
#define IGNIS_IPRINTER_H

#include "IEnumTypes.h"
#include "ignis/executor/core/RTTInfo.h"
#include "ignis/executor/core/protocol/IProtocol.h"
#include "ignis/executor/core/exception/ILogicError.h"
#include <ostream>

namespace ignis {
    namespace executor {
        namespace core {
            namespace io {

                std::string inline tab(size_t n) { return std::string(n, '\t'); }

                template<typename T>
                struct IPrinterType {
                    inline void operator()(const T &b, std::ostream &out, size_t level) {
                        throw exception::ILogicError(
                                "IPrinterType not implemented for " + RTTInfo::from<T>().getStandardName());
                    }
                };

                template<typename T>
                class IPrinter {
                public:
                    virtual void operator()(const T &b, std::ostream &out) {
                        printer(b, out, 0);
                    }

                private:
                    IPrinterType<T> printer;
                };
            }
        }
    }
}

#include "IPrinter.tcc"
#endif
