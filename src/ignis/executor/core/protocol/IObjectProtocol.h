
#ifndef IGNIS_IOBJECTPROTOCOL_H
#define IGNIS_IOBJECTPROTOCOL_H

#include "IProtocol.h"
#include "ignis/executor/core/io/IReader.h"
#include "ignis/executor/core/io/IWriter.h"
#include "ignis/executor/core/io/INativeReader.h"
#include "ignis/executor/core/io/INativeWriter.h"
#include "ignis/executor/core/transport/ITransport.h"
#include "ignis/executor/core/exception/IInvalidArgument.h"
#include "ignis/executor/core/exception/ILogicError.h"
#include <thrift/protocol/TProtocolDecorator.h>
#include <thrift/protocol/TCompactProtocol.h>
#include <vector>

namespace ignis {
    namespace executor {
        namespace core {
            namespace protocol {

                class IObjectProtocol : public apache::thrift::protocol::TProtocolDecorator {
                public:

                    const static int8_t IGNIS_PROTOCOL = 0;
                    const static int8_t CPP_PROTOCOL = 1;

                    IObjectProtocol(const std::shared_ptr<transport::ITransport> &trans) :
                            TProtocolDecorator(std::make_shared<apache::thrift::protocol::TCompactProtocol>(trans)) {
                    }

                    template<typename Tp>
                    Tp readObject() {
                        Tp obj;
                        readObject(obj);
                        return obj;
                    }

                    template<typename Tp>
                    void readObject(Tp &obj) {
                        if (readSerialization()) {
                            io::INativeReader<Tp> reader;
                            reader(*this, obj);
                        } else {
                            io::IReader<Tp> reader;
                            reader.readType(*this);
                            reader(*this, obj);
                        }
                    }

                    template<typename Tp>
                    void writeObject(const Tp &obj, bool native = false) {
                        writeSerialization(native);
                        if (native) {
                            io::INativeWriter<Tp> writer;
                            writer(*this, obj);
                        } else {
                            io::IWriter<Tp> writer;
                            writer.writeType(*this);
                            writer(*this, obj);
                        }
                    }

                    bool readSerialization(bool acceptNative = false) {
                        int8_t id;
                        this->readByte(id);
                        if (id == IGNIS_PROTOCOL) {
                            return false;
                        } else if (id == CPP_PROTOCOL) {
                            if (!acceptNative) {
                                throw exception::ILogicError("Native serialization doesn't support");
                            }
                            return true;
                        } else {
                            throw exception::ILogicError("Serialization is not compatible with C++");
                        }
                    }

                    void writeSerialization(bool native = false) {
                        if (native) {
                            writeByte(CPP_PROTOCOL);
                        } else {
                            writeByte(IGNIS_PROTOCOL);
                        }
                    }

                    virtual ~IObjectProtocol() {}

                };
            }
        }
    }
}

#endif

