
#ifndef IGNIS_IOBJECT_H
#define IGNIS_IOBJECT_H

#include <memory>
#include <thrift/transport/TTransport.h>
#include "../../../data/serialization/ITypeHandle.h"
#include "../../api/IReadIterator.h"
#include "../../api/IWriteIterator.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {
                class IObject {
                public:
                    typedef char Any;

                    IObject() {}

                    virtual ~IObject() {}

                    virtual std::shared_ptr<api::IReadIterator<Any>> readIterator() = 0;

                    virtual std::shared_ptr<api::IWriteIterator<Any>> writeIterator() = 0;

                    virtual void read(std::shared_ptr<apache::thrift::transport::TTransport> trans) = 0;

                    virtual void write(std::shared_ptr<apache::thrift::transport::TTransport> trans, int8_t compression) = 0;

                    void setTypeHandle(std::shared_ptr<data::serialization::ITypeHandleBase<Any>> type_handle);

                protected:
                    std::shared_ptr<data::serialization::ITypeHandleBase<Any>> type_handle;

                };
            }
        }
    }
}

#endif
