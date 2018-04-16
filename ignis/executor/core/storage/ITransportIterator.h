
#ifndef IGNIS_ITRANSPORTITERATOR_H
#define IGNIS_ITRANSPORTITERATOR_H

#include <thrift/transport/TTransport.h>
#include "../../../data/IObjectProtocol.h"
#include "IObject.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {
                class IReadTransportIterator : public api::IReadIterator<IObject::Any> {
                public:

                    IReadTransportIterator(const std::shared_ptr<apache::thrift::transport::TTransport> &transport,
                                            const std::shared_ptr<data::serialization::ITypeHandleBase<IObject::Any>> &type_handle,
                                            size_t elems);

                    IObject::Any &next() override;

                    bool hashNext() override;

                    virtual ~IReadTransportIterator();

                private:
                    std::shared_ptr<apache::thrift::transport::TTransport> transport;
                    std::shared_ptr<ignis::data::IObjectProtocol> protocol;
                    std::shared_ptr<ignis::data::serialization::ITypeHandleBase<IObject::Any>> type_handle;
                    std::shared_ptr<ignis::data::serialization::IReader<IObject::Any>> reader;
                    std::shared_ptr<IObject::Handle<IObject::Any>> actual;
                    size_t elems;
                };

                class IWriteTransportIterator : public api::IWriteIterator<IObject::Any> {
                public:

                    IWriteTransportIterator(const std::shared_ptr<apache::thrift::transport::TTransport> &transport,
                                            const std::shared_ptr<data::serialization::ITypeHandleBase<IObject::Any>> &type_handle,
                                            size_t &elems);

                    void write(IObject::Any &obj) override;

                    void write(IObject::Any &&obj) override;

                    virtual ~IWriteTransportIterator();

                private:
                    std::shared_ptr<apache::thrift::transport::TTransport> transport;
                    std::shared_ptr<ignis::data::IObjectProtocol> protocol;
                    std::shared_ptr<ignis::data::serialization::ITypeHandleBase<IObject::Any>> type_handle;
                    std::shared_ptr<ignis::data::serialization::IWriter<IObject::Any>> writer;
                    size_t &elems;
                };
            }
        }
    }
}

#endif
