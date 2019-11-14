
#ifndef IGNIS_IREADER_H
#define IGNIS_IREADER_H

#include "IEnumTypes.h"
#include "executor/core/RTTInfo.h"
#include "executor/core/protocol/IProtocol.h"
#include "executor/core/exception/ILogicError.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace io {
                inline int8_t readTypeAux(protocol::IProtocol &protocol) {
                    int8_t type;
                    protocol.readByte(type);
                    return type;
                }

                inline int64_t readSizeAux(protocol::IProtocol &protocol) {
                    int64_t size;
                    protocol.readI64(size);
                    return size;
                }

                template<typename T>
                inline void checkTypeAux(bool readType) {
                    if (!readType) {
                        throw exception::ILogicError(
                                "IReaderType type error for " + RTTInfo::from<T>().getStandardName());
                    }
                }

                template<typename T>
                struct IReaderType {

                    virtual bool readType(protocol::IProtocol &protocol) {
                        throw exception::ILogicError(
                                "IReaderType not implemented for " + RTTInfo(typeid(T)).getStandardName());
                    }

                    virtual void operator()(protocol::IProtocol &protocol, T& obj){
                        throw exception::ILogicError(
                                "IReaderType not implemented for " + RTTInfo(typeid(T)).getStandardName());
                    }

                    virtual T operator()(protocol::IProtocol &protocol) {
                        throw exception::ILogicError(
                                "IReaderType not implemented for " + RTTInfo(typeid(T)).getStandardName());
                    }
                };

                template<typename T>
                class IReader {
                public:

                    virtual inline T operator()(protocol::IProtocol &protocol) {
                        return reader(protocol);
                    }

                    virtual inline void operator()(protocol::IProtocol &protocol, T& obj) {
                        reader(protocol, obj);
                    }

                    virtual inline void readType(protocol::IProtocol &protocol) {
                        checkTypeAux<T>(reader.readType(protocol));
                    }

                private:
                    IReaderType<T> reader;

                };

            }
        }
    }
}

#include "IReader.tcc"
#endif
