
#include "IMemoryObject.h"
#include <thrift/transport/TZlibTransport.h>
#include "../../../data/IObjectProtocol.h"
#include "iterator/IMemoryIterator.h"

using namespace ignis::executor::core::storage;
using namespace ignis::data;

IMemoryObject::IMemoryObject(const std::shared_ptr<api::IManager<Any>> &manager, size_t size)
        : data(new std::vector<IObject::Any>()), collection_manager(manager->collectionManager()) {
    setManager(manager);
}

std::shared_ptr<iterator::ICoreReadIterator<IObject::Any>> IMemoryObject::readIterator() {
    return std::make_shared<iterator::IReadMemoryIterator>(data, collection_manager);
}

std::shared_ptr<iterator::ICoreWriteIterator<IObject::Any>> IMemoryObject::writeIterator() {
    return std::make_shared<iterator::IWriteMemoryIterator>(data, collection_manager);
}

void IMemoryObject::read(std::shared_ptr<transport::TTransport> trans) {
    auto data_transport = std::make_shared<transport::TZlibTransport>(trans);
    data::IObjectProtocol data_proto(data_transport);
    auto reader = collection_manager->reader();
    clear();
    delete data;
    data = reader->readPtr(data_proto);
}

void IMemoryObject::write(std::shared_ptr<transport::TTransport> trans, int8_t compression) {
    auto data_transport = std::make_shared<transport::TZlibTransport>(trans, 128, 1024, 128, 1024, compression);
    data::IObjectProtocol data_proto(data_transport);
    auto writer = collection_manager->writer();
    writer->writePtr(data, data_proto);
}

void IMemoryObject::copyFrom(IObject &source) {
    iterator::readToWrite(*(source.readIterator()), *(this->writeIterator()));
}

void IMemoryObject::moveFrom(IObject &source) {
    iterator::readToWrite(*(source.readIterator()), *(this->writeIterator()), true);
}

size_t IMemoryObject::getSize() {
    return collection_manager->size(*data);
}

void IMemoryObject::clear() {
    collection_manager->clear(*data);
}

void IMemoryObject::fit() {
    collection_manager->resize(*data, collection_manager->size(*data));
}

std::string IMemoryObject::getType() {
    return "memory";
}

IMemoryObject::~IMemoryObject() {
    clear();
    delete data;
}

