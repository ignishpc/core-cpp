
#ifndef IGNIS_ICOMPATIBILITYEXCEPTION_H
#define IGNIS_ICOMPATIBILITYEXCEPTION_H

#include "IException.h"
#include "ignis/executor/core/RTTInfo.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace exception {
                class ICompatibilyException : public IException {
                public:
                    ICompatibilyException(const std::string &method, const RTTInfo &function)
                            : IException(msg(method, function)) {}

                    virtual ~ICompatibilyException() {}

                private:

                    std::string base(const RTTInfo &function) {
                        std::string str = function.getStandardName();
                        int bases = function.getNumOfBases();
                        if (bases == 1) {
                            return str + " : " + base(RTTInfo(function.getBase()));
                        } else if (bases > 1) {
                            str += " : (" + base(RTTInfo(function.getBase(0)));
                            for (int i = 1; i < bases; i++) {
                                str += ", " + base(RTTInfo(function.getBase(i)));
                            }
                            return str + ")";
                        }
                        return str;
                    }

                    std::string msg(const std::string &method, const RTTInfo &function) {
                        return base(function) + " is not compatible with " + method;
                    }
                };
            }
        }
    }
}

#endif

