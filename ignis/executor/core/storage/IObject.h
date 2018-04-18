
#ifndef IGNIS_IOBJECT_H
#define IGNIS_IOBJECT_H

#include <memory>
#include <thrift/transport/TTransport.h>
#include "../../../data/IManager.h"
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

                    virtual std::shared_ptr<data::IManager<Any>> getManager() {
                        return manager;
                    }

                    virtual void setManager(std::shared_ptr<data::IManager<Any>> manager) {
                        this->manager = manager;
                    }

                    template<typename T>
                    inline static Any &toAny(T &obj) {
                        return (Any &) obj;
                    }

                    template<typename T = Any>
                    class Handle {
                    public:
                        Handle(T *ptr,
                               const std::shared_ptr<data::serialization::ITypeHandle<T>> &type_handle) :
                                ptr(ptr), type_handle(type_handle) {}

                        virtual ~Handle() { (*type_handle->deleter())(ptr); }

                    private:
                        T *ptr;
                        std::shared_ptr<data::serialization::ITypeHandle<T>> type_handle;
                    };

                protected:

                    std::shared_ptr<data::IManager<Any>> manager;

                };
            }
        }
    }
}

#endif
