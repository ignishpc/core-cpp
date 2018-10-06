
#include "ITransportIterator.h"

using namespace ignis::data;
using namespace ignis::executor::core::storage;
using namespace ignis::executor::core::storage::iterator;

IReadTransportIterator::IReadTransportIterator(const std::shared_ptr<transport::TTransport> &transport,
                                               const std::shared_ptr<IManager<IObject::Any>> &manager,
                                               size_t elems)
        : transport(transport),
          type_handle(manager->getClassManagerType()->getElemClassManager()->getTypeHandle()),
          elems(elems),
          reader(type_handle->reader()),
          protocol(std::make_shared<data::IObjectProtocol>(transport)) {}

IWriteTransportIterator::IWriteTransportIterator(const std::shared_ptr<transport::TTransport> &transport,
                                                 const std::shared_ptr<IManager<IObject::Any>> &manager,
                                                 size_t &elems)
        : transport(transport),
          type_handle(manager->getClassManagerType()->getElemClassManager()->getTypeHandle()),
          elems(elems),
          writer(type_handle->writer()),
          protocol(std::make_shared<data::IObjectProtocol>(transport)) {}

IObject::Any &IReadTransportIterator::next() {
    elems--;
    auto obj = reader->readPtr(*protocol);
    actual = std::make_shared<IObject::Handle<IObject::Any>>(obj, type_handle);
    return *obj;
}

bool IReadTransportIterator::hashNext() {
    elems > 0;
}

IReadTransportIterator::~IReadTransportIterator() {
}

bool IReadTransportIterator::isMoved() {
    return true;
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


