
#include "ITransportIterator.h"

using namespace ignis::executor::core::storage;
using namespace ignis::executor::core::storage::iterator;

IReadTransportIterator::IReadTransportIterator(const std::shared_ptr<transport::TTransport> &transport,
                                               const std::shared_ptr<api::IManager<IObject::Any>> &manager,
                                               size_t &elems)
        : transport(transport),
          protocol(std::make_shared<data::IObjectProtocol>(transport)),
          manager(manager),
          elems(elems),
          reader(manager->reader()) {}

IWriteTransportIterator::IWriteTransportIterator(const std::shared_ptr<transport::TTransport> &transport,
                                                 const std::shared_ptr<api::IManager<IObject::Any>> &manager,
                                                 size_t &elems)
        : transport(transport),
          protocol(std::make_shared<data::IObjectProtocol>(transport)),
          manager(manager),
          elems(elems),
          writer(manager->writer()){}

IObject::Any &IReadTransportIterator::next() {
    elems--;
    auto obj = reader->readPtr(*protocol);
    actual = std::make_shared<IObject::DataHandle<IObject::Any>>(obj, manager);
    return *obj;
}

bool IReadTransportIterator::hasNext() {
    return elems > 0;
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


