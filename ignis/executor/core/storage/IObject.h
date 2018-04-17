
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

                    virtual ~IObject() {};

                    virtual std::shared_ptr<api::IReadIterator<Any>> readIterator() = 0;

                    virtual std::shared_ptr<api::IWriteIterator<Any>> writeIterator() = 0;

                    virtual void read(std::shared_ptr<apache::thrift::transport::TTransport> trans) = 0;

                    virtual void
                    write(std::shared_ptr<apache::thrift::transport::TTransport> trans, int8_t compression) = 0;

                    virtual size_t getSize() = 0;

                    virtual std::shared_ptr<data::serialization::ITypeHandleBase<Any>> getTypeHandle(){
                        return type_handle;
                    }

                    virtual void setTypeHandle(std::shared_ptr<data::serialization::ITypeHandleBase<Any>> type_handle) {
                        this->type_handle = type_handle;
                    }

                    template<typename T>
                    static inline std::shared_ptr<data::serialization::ITypeHandleBase<Any>>& castTypeHandle(
                            const std::shared_ptr<data::serialization::ITypeHandleBase<T>> &type_handle){
                        return (std::shared_ptr<data::serialization::ITypeHandleBase<IObject::Any>>&)type_handle;
                    }

                    template<typename T>
                    static inline Any& toAny(T &obj){
                        return (Any&)obj;
                    }

                    template<typename T = Any>
                    class Handle {
                    public:
                        Handle(T *ptr,
                               const std::shared_ptr<data::serialization::ITypeHandleBase<T>> &type_handle) :
                                ptr(ptr), type_handle(type_handle) {}

                        virtual ~Handle() { (*type_handle->deleter())(ptr); }

                    protected:
                        T *ptr;
                        std::shared_ptr<data::serialization::ITypeHandleBase<T>> type_handle;
                    };

                protected:

                    std::shared_ptr<data::serialization::ITypeHandleBase<Any>> type_handle;

                };
            }
        }
    }
}

#endif
