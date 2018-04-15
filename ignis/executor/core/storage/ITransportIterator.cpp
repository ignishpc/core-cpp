
#include "ITransportIterator.h"

using namespace std;
using namespace ignis::executor::core::storage;
using namespace ignis::data::serialization;
using namespace apache::thrift::transport;

IReadTransportIterator::IReadTransportIterator(const shared_ptr<TTransport> &transport,
                                               const shared_ptr<ITypeHandleBase<IObject::Any>> &type_handle,
                                               size_t elems)
        : transport(transport),
          type_handle(type_handle),
          elems(elems),
          reader(type_handle->reader()),
          protocol(make_shared<data::IObjectProtocol>(transport)) {}

IWriteTransportIterator::IWriteTransportIterator(const shared_ptr<TTransport> &transport,
                                                 const shared_ptr<ITypeHandleBase<IObject::Any>> &type_handle,
                                                 size_t &elems)
        : transport(transport),
          type_handle(type_handle),
          elems(elems),
          writer(type_handle->writer()),
          protocol(make_shared<data::IObjectProtocol>(transport)) {}

IReadTransportIterator::Handle::Handle(IObject::Any *ptr, const shared_ptr<ITypeHandleBase<IObject::Any>> &type_handle)
        : ptr(ptr), type_handle(type_handle) {}

IReadTransportIterator::Handle::~Handle(){
    (*type_handle->deleter())(ptr);
};

IObject::Any &IReadTransportIterator::next() {
    elems--;
    auto obj = reader->readPtr(*protocol);
    actual = make_shared<IReadTransportIterator::Handle>(obj, type_handle);
    return *obj;
}

bool IReadTransportIterator::hashNext() {
    elems > 0;
}

void IWriteTransportIterator::write(IObject::Any &obj) {
    elems++;
    (*writer)(obj, *protocol);
}

void IWriteTransportIterator::write(IObject::Any &&obj) {
    elems++;
    (*writer)(obj, *protocol);
}


