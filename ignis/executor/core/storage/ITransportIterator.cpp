
#include "ITransportIterator.h"

using namespace std;
using namespace ignis::executor::core::storage;
using namespace ignis::data;
using namespace apache::thrift::transport;

IReadTransportIterator::IReadTransportIterator(const shared_ptr<TTransport> &transport,
                                               const shared_ptr<IManager<IObject::Any>> &manager,
                                               size_t elems)
        : transport(transport),
          type_handle(manager->getClassManagerType()->getElemClassManager()->getTypeHandle()),
          elems(elems),
          reader(type_handle->reader()),
          protocol(make_shared<data::IObjectProtocol>(transport)) {}

IWriteTransportIterator::IWriteTransportIterator(const shared_ptr<TTransport> &transport,
                                                 const shared_ptr<IManager<IObject::Any>> &manager,
                                                 size_t &elems)
        : transport(transport),
          type_handle(manager->getClassManagerType()->getElemClassManager()->getTypeHandle()),
          elems(elems),
          writer(type_handle->writer()),
          protocol(make_shared<data::IObjectProtocol>(transport)) {}

IObject::Any &IReadTransportIterator::next() {
    elems--;
    auto obj = reader->readPtr(*protocol);
    actual = make_shared<IObject::Handle<IObject::Any>>(obj, type_handle);
    return *obj;
}

bool IReadTransportIterator::hashNext() {
    elems > 0;
}

IReadTransportIterator::~IReadTransportIterator() {
}

void IWriteTransportIterator::write(IObject::Any &obj) {
    elems++;
    (*writer)(obj, *protocol);
}

void IWriteTransportIterator::write(IObject::Any &&obj) {
    elems++;
    (*writer)(obj, *protocol);
}

IWriteTransportIterator::~IWriteTransportIterator() {
}


