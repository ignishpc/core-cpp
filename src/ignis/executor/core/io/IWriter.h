
#ifndef IGNIS_IWRITER_H
#define IGNIS_IWRITER_H


#include "IEnumTypes.h"
#include "../RTTInfo.h"
#include "../protocol/IProtocol.h"
#include "../exception/ILogicError.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace io {

                inline void writeSizeAux(protocol::IProtocol &protocol, int64_t size) {
                    protocol.writeI64(size);
                }

                inline void writeTypeAux(protocol::IProtocol &protocol, int8_t tp) {
                    protocol.writeByte(tp);
                }

                template<typename T>
                struct IWriterType {

                    void writeType(protocol::IProtocol &protocol) {
                        throw exception::ILogicError(
                                "IWriterType not implemented for " + RTTInfo::from<T>().getStandardName());
                    }

                    void operator()(const T &obj, protocol::IProtocol &protocol) {
                        throw exception::ILogicError(
                                "IWriterType not implemented for " + RTTInfo::from<T>().getStandardName());
                    }
                };

                template<typename T>
                class IWriter {
                public:
                    virtual void writeType(protocol::IProtocol &protocol) {
                        writer.writeType(protocol);
                    }

                    virtual void operator()(const T &obj, protocol::IProtocol &protocol) {
                        writer(obj, protocol);
                    }

                private:
                    IWriterType<T> writer;
                };
            }
        }
    }
}

#include "IWriter.tcc"

#endif

