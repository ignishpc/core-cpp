
#ifndef IGNIS_IOBJECTPROTOCOL_H
#define IGNIS_IOBJECTPROTOCOL_H

#include "IProtocol.h"
#include "ignis/executor/core/io/IReader.h"
#include "ignis/executor/core/io/IWriter.h"
#include "ignis/executor/core/transport/ITransport.h"
#include "ignis/executor/core/exception/IInvalidArgument.h"
#include "ignis/executor/core/exception/ILogicError.h"
#include <thrift/protocol/TProtocolDecorator.h>
#include <thrift/protocol/TCompactProtocol.h>

namespace ignis {
    namespace executor {
        namespace core {
            namespace protocol {
                class IObjectProtocol : public apache::thrift::protocol::TProtocolDecorator {
                public:

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
                        readSerialization();
                        io::IReader<Tp> reader;
                        reader.readType(*this);
                        return reader(*this, obj);
                    }

                    template<typename Tp>
                    void writeObject(const Tp &obj) {
                        io::IWriter<Tp> writer;
                        writeSerialization();
                        writer.writeType(*this);
                        writer(*this, obj);
                    }

                    void readSerialization(){
                        int8_t id;
                        this->readByte(id);
                        if (id != 0){
                            throw exception::ILogicError("C++ only support Ignis serialization");
                        }
                    }

                    void writeSerialization(){
                        this->writeByte(0);
                    }

                    virtual ~IObjectProtocol() {}
                };
            }
        }
    }
}

#endif

