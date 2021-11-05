
#ifndef IGNIS_INATIVEREADER_H
#define IGNIS_INATIVEREADER_H

#include "IReader.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace io {

                template<typename T>
                inline std::string checkNativeType(protocol::IProtocol &protocol) {
                    std::string type;
                    protocol.readBinary(type);
                    if (RTTInfo::from<T>().getStandardName() != type) {
                        throw exception::ILogicError("IReaderType type error for " +
                                                     RTTInfo::from<T>().getStandardName());
                    }
                    return type;
                }

                template<typename T>
                struct INativeReaderType;

                template<typename T>
                class INativeReader {
                public:
                    virtual inline T operator()(protocol::IProtocol &protocol) { return reader(protocol); }

                    virtual inline void operator()(protocol::IProtocol &protocol, T &obj) { reader(protocol, obj); }

                private:
                    INativeReaderType<T> reader;
                };
            }// namespace io
        }    // namespace core
    }        // namespace executor
}// namespace ignis

#include "INativeReader.tcc"

#endif
