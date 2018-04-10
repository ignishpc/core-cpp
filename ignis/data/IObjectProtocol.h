
#ifndef IGNIS_IOBJECTPROTOCOL_H
#define IGNIS_IOBJECTPROTOCOL_H

#include <memory>
#include <thrift/transport/TTransport.h>
#include <thrift/protocol/TProtocolDecorator.h>
#include <thrift/protocol/TCompactProtocol.h>
#include "serialization/IReader.h"
#include "serialization/IWriter.h"

namespace ignis {
    namespace data {
        class IObjectProtocol : public apache::thrift::protocol::TProtocolDecorator {

        public:

            IObjectProtocol(const std::shared_ptr<apache::thrift::transport::TTransport> &trans) :
                    TProtocolDecorator(std::make_shared<apache::thrift::protocol::TCompactProtocol>(trans)) {
            }

            template<typename T>
            T readObject(const serialization::IReader<T> &reader) {
                bool native;
                this->readBool(native);
                reader.checkType(*this);
                reader.readType(*this);
            }

            template<typename T>
            std::shared_ptr<T> readSharedObject(const serialization::IReader<T> &reader) {
                return std::make_shared<T>(new T(readObject(reader)));
            }

            template<typename T>
            void writeObject(const T &obj, const serialization::IWriter<T> &writer) {
                this->writeBool(false);
                writer.writeType(*this);
                writer(obj, *this);
            }

        };
    }
}

#endif
