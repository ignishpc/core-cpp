
#ifndef IGNIS_IMEMORYOBJECT_H
#define IGNIS_IMEMORYOBJECT_H

#include "IObject.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {
                class IMemoryObject : public IObject {

                public:

                    IMemoryObject(const std::shared_ptr<data::serialization::ITypeHandleBase<Any>> &type_handle, uint32_t sz = 1000);

                    IMemoryObject(std::vector<IObject::Any>* data, const std::shared_ptr<data::serialization::ITypeHandleBase<Any>> &type_handle);

                    std::shared_ptr<api::IReadIterator<Any>> readIterator() override;

                    std::shared_ptr<api::IWriteIterator<Any>> writeIterator() override;

                    void read(std::shared_ptr<apache::thrift::transport::TTransport> trans) override;

                    void
                    write(std::shared_ptr<apache::thrift::transport::TTransport> trans, int8_t compression) override;

                    size_t getSize() override;

                    virtual ~IMemoryObject();

                private:
                    std::vector<IObject::Any> *data;
                    std::shared_ptr<data::serialization::IColectionHandle<IObject::Any>> data_handle;
                    std::shared_ptr<Handle<std::vector<IObject::Any>>> handle;

                };
            }
        }
    }
}

#endif
