
#ifndef IGNIS_IJSONPRINTER_H
#define IGNIS_IJSONPRINTER_H

#include "IEnumTypes.h"
#include "../RTTInfo.h"
#include "../protocol/IProtocol.h"
#include "../exceptions/ILogicError.h"
#include <ostream>
#include <rapidjson/prettywriter.h>
#include <rapidjson/ostreamwrapper.h>

namespace ignis {
    namespace executor {
        namespace core {
            namespace io {

                typedef rapidjson::Writer<rapidjson::OStreamWrapper> JsonStream;

                template<typename T>
                struct IJsonPrinterType {
                    virtual void operator()(const T &b, JsonStream &out) {
                        throw exceptions::ILogicError(
                                "IJsonPrinterType not implemented for " + RTTInfo::from<T>().getStandardName());
                    }
                };

                template<typename T>
                class IJsonPrinter {
                public:
                          virtual void operator()(const T &b, std::ostream &out, bool pretty = true) {
                        rapidjson::OStreamWrapper wrapper(out);
                        if (pretty) {
                            rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(wrapper);
                            printer(b, (JsonStream &) writer);

                        } else {
                            JsonStream writer(wrapper);
                            printer(b, writer);
                        }
                    }

                private:
                    IJsonPrinterType <T> printer;
                };
            }
        }
    }
}

#include "IJsonPrinter.tcc"
#endif
