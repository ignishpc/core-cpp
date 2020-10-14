
#ifndef IGNIS_IPRINTER_H
#define IGNIS_IPRINTER_H

#include "IEnumTypes.h"
#include "ignis/executor/core/RTTInfo.h"
#include "ignis/executor/core/exception/ILogicError.h"
#include "ignis/executor/core/protocol/IProtocol.h"
#include <ostream>

namespace ignis {
    namespace executor {
        namespace core {
            namespace io {

                std::string inline tab(int64_t n) { return std::string(n > 0 ? n : 0, '\t'); }

                template<typename T>
                struct IPrinterType {
                    inline void operator()(std::ostream &out, const T &b, int64_t level) {
                        throw exception::ILogicError("IPrinterType not implemented for " +
                                                     RTTInfo::from<T>().getStandardName());
                    }
                };

                template<typename T>
                class IPrinter {
                public:
                    virtual void operator()(std::ostream &out, const T &b) {
                        printer(out, b, -1);//-1 no ident first array
                    }

                private:
                    IPrinterType<T> printer;
                };
            }// namespace io
        }    // namespace core
    }        // namespace executor
}// namespace ignis

#include "IPrinter.tcc"
#endif
