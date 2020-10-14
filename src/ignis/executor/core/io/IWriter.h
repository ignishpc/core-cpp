
#ifndef IGNIS_IWRITER_H
#define IGNIS_IWRITER_H


#include "../RTTInfo.h"
#include "../exception/ILogicError.h"
#include "../protocol/IProtocol.h"
#include "IEnumTypes.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace io {

                inline void writeSizeAux(protocol::IProtocol &protocol, int64_t size) { protocol.writeI64(size); }

                inline void writeTypeAux(protocol::IProtocol &protocol, int8_t tp) { protocol.writeByte(tp); }

                template<typename T>
                struct IWriterType {

                    inline void writeType(protocol::IProtocol &protocol) {
                        throw exception::ILogicError("IWriterType not implemented for " +
                                                     RTTInfo::from<T>().getStandardName());
                    }

                    inline void operator()(protocol::IProtocol &protocol, const T &obj) {
                        throw exception::ILogicError("IWriterType not implemented for " +
                                                     RTTInfo::from<T>().getStandardName());
                    }
                };

                template<typename T>
                class IWriter {
                public:
                    virtual void writeType(protocol::IProtocol &protocol) { writer.writeType(protocol); }

                    virtual void operator()(protocol::IProtocol &protocol, const T &obj) { writer(protocol, obj); }

                private:
                    IWriterType<T> writer;
                };
            }// namespace io
        }    // namespace core
    }        // namespace executor
}// namespace ignis

#include "IWriter.tcc"

#endif
