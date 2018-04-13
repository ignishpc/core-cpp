
#include "IObject.h"

using namespace ignis::executor::core::storage;

IObject::Handle::Handle(Any *ptr, const std::shared_ptr<ignis::data::serialization::ITypeHandleBase<Any>> &type_handle)
        : ptr(ptr), type_handle(type_handle) {}

IObject::Any *IObject::Handle::data() { return ptr; }

IObject::Handle::~Handle() { type_handle->deleter()->deleteObject(ptr); }

void IObject::setTypeHandle(std::shared_ptr<ignis::data::serialization::ITypeHandleBase<IObject::Any>> &type_handle) {
    this->type_handle = type_handle;
}



