
#ifndef IGNIS_IRAWMEMORYOBJECT_H
#define IGNIS_IRAWMEMORYOBJECT_H

#include <memory>
#include <thrift/transport/TBufferTransports.h>
#include "IObject.h"
#include "../../../data/IObjectProtocol.h"


namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {
                class IRawMemoryObject : public IObject {
                public:
                    IRawMemoryObject();

                    IRawMemoryObject(const std::shared_ptr<apache::thrift::transport::TMemoryBuffer> &raw_memory);

                    std::shared_ptr<api::IReadIterator<Any>> readIterator() override;

                    std::shared_ptr<api::IWriteIterator<Any>> writeIterator() override;

                    std::shared_ptr<Handle> getObject() override;

                    void write(std::shared_ptr<apache::thrift::transport::TTransport> &trans) override;

                private:
                    size_t elems;
                    size_t bytes;

                    void writeHeader();

                    std::shared_ptr<apache::thrift::transport::TMemoryBuffer> raw_memory;
                };

                class IRawReadIterator : public api::IReadIterator<IObject::Any> {
                public:
                    IRawReadIterator(const std::shared_ptr<apache::thrift::transport::TMemoryBuffer> &raw_memory,
                                     const std::shared_ptr<data::serialization::ITypeHandleBase<IObject::Any>> &type_handle);

                    bool hashNext() override;

                    IObject::Any *next() override;

                private:
                    std::shared_ptr<apache::thrift::transport::TMemoryBuffer> raw_memory;
                    std::shared_ptr<ignis::data::IObjectProtocol> protocol;
                    std::shared_ptr<ignis::data::serialization::ITypeHandleBase<IObject::Any>> type_handle;
                    std::shared_ptr<ignis::data::serialization::IReader<IObject::Any>> reader;
                    std::shared_ptr<IObject::Handle> actual;
                    size_t size;
                    size_t read;
                };


                class IRawWriteIterator : public api::IWriteIterator<IObject::Any> {
                public:
                    IRawWriteIterator(const std::shared_ptr<apache::thrift::transport::TMemoryBuffer> &raw_memory,
                                      const std::shared_ptr<data::serialization::ITypeHandleBase<IObject::Any>> &type_handle,
                                      size_t &elems);

                    void write(IObject::Any &obj) override;

                    void write(IObject::Any &&obj) override;

                private:
                    std::shared_ptr<apache::thrift::transport::TMemoryBuffer> raw_memory;
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
