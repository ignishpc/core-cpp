
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
            std::shared_ptr<T> readObject(serialization::IReader<T> &reader) {
                bool native;
                this->readBool(native);
                return reader.readShared(*this);
            }

            template<typename T>
            void writeObject(const T &obj, serialization::IWriter<T> &writer) {
                this->writeBool(false);
                writer(obj, *this);
            }

            virtual ~IObjectProtocol() {}

        };
    }
}

#endif
