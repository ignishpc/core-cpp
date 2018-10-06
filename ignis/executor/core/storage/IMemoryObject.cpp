
#include "IMemoryObject.h"
#include <thrift/transport/TZlibTransport.h>
#include "../../../data/IObjectProtocol.h"
#include "iterator/IMemoryIterator.h"

using namespace ignis::executor::core::storage;
using namespace ignis::data::serialization;

IMemoryObject::IMemoryObject(const std::shared_ptr<data::IManager<Any>> &manager, size_t size)
        : manager(manager),
          data(new std::vector<IObject::Any>()),
          handle(std::make_shared<IObject::Handle<std::vector<IObject::Any>>>(data,
                                                                              manager->getClassManagerType()->getTypeHandle())) {}

std::shared_ptr<iterator::ICoreReadIterator<IObject::Any>> IMemoryObject::readIterator() {
    return std::make_shared<iterator::IReadMemoryIterator>(data, manager);
}

std::shared_ptr<iterator::ICoreWriteIterator<IObject::Any>> IMemoryObject::writeIterator() {
    return std::make_shared<iterator::IWriteMemoryIterator>(data, manager);
}

void IMemoryObject::read(std::shared_ptr<transport::TTransport> trans) {
    auto data_transport = std::make_shared<transport::TZlibTransport>(trans);
    data::IObjectProtocol data_proto(data_transport);
    auto type_handle = manager->getClassManagerType()->getTypeHandle();
    auto reader = type_handle->reader();
    data = reader->readPtr(data_proto);
    handle = std::make_shared<IObject::Handle<std::vector<IObject::Any>>>(data, type_handle);
}

void IMemoryObject::write(std::shared_ptr<transport::TTransport> trans, int8_t compression) {
    auto data_transport = std::make_shared<transport::TZlibTransport>(trans, 128, 1024, 128, 1024, compression);
    data::IObjectProtocol data_proto(data_transport);
    auto writer = manager->getClassManagerType()->getTypeHandle()->writer();
    writer->writePtr(data, data_proto);
}

void IMemoryObject::copyFrom(IObject &source) {
    iterator::readToWrite(*(source.readIterator()), *(this->writeIterator()));
}

void IMemoryObject::moveFrom(IObject &source) {
    iterator::readToWrite(*(source.readIterator()), *(this->writeIterator()), true);
}

size_t IMemoryObject::getSize() {
    return manager->getClassManagerType()->size(*data);
}

void IMemoryObject::clear() {
    manager->getClassManagerType()->clear(*data);
}

void IMemoryObject::fit() {
    manager->getClassManagerType()->resize(*data, manager->getClassManagerType()->size(*data));
}

std::string IMemoryObject::getType() {
    return "memory";
}

IMemoryObject::~IMemoryObject() {

}


