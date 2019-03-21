
#ifndef IGNIS_IOBJECTPROTOCOL_H
#define IGNIS_IOBJECTPROTOCOL_H

#include <memory>
#include <thrift/transport/TTransport.h>
#include <thrift/protocol/TProtocolDecorator.h>
#include <thrift/protocol/TCompactProtocol.h>
#include "handle/IReader.h"
#include "handle/IDeleter.h"
#include "handle/IWriter.h"
#include "../exceptions/IInvalidArgument.h"

namespace ignis {
    namespace data {
        class IObjectProtocol : public apache::thrift::protocol::TProtocolDecorator {

        public:

            IObjectProtocol(const std::shared_ptr<apache::thrift::transport::TTransport> &trans) :
                    TProtocolDecorator(std::make_shared<apache::thrift::protocol::TCompactProtocol>(trans)) {
            }

            std::shared_ptr<void> readObject(handle::IReader<void> &reader);//Protect void template implementation

            template<typename T>
            std::shared_ptr<T> readObject(handle::IReader<T> &reader, std::shared_ptr<handle::IDeleter<T>> deleter) {
                return std::shared_ptr<T>(readPtrObject(reader),[deleter](T* ptr){(*deleter)(ptr);});
            }

            template<typename T>
            std::shared_ptr<T> readObject(handle::IReader<T> &reader) {
                return std::shared_ptr<T>(readPtrObject(reader));
            }

            template<typename T>
            T* readPtrObject(handle::IReader<T> &reader) {
                bool native;
                this->readBool(native);
                if(native){
                    throw exceptions::IInvalidArgument("C++ does not have a native serialization");
                }
                reader.readType(*this);
                return reader.readPtr(*this);
            }

            template<typename T>
            void writeObject(const T &obj, handle::IWriter<T> &writer) {
                this->writeBool(false);
                writer.writeType(*this);
                writer(obj, *this);
            }

            virtual ~IObjectProtocol() {}

        };
    }
}

#endif
