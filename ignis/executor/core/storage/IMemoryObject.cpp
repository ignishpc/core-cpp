
#include "IMemoryObject.h"
#include "../../../data/IZlibTransport.h"
#include "../../../data/IObjectProtocol.h"
#include "iterator/IMemoryIterator.h"

using namespace ignis::executor::core::storage;
using namespace ignis::data;

const std::string IMemoryObject::TYPE = "memory";

IMemoryObject::IMemoryObject(const std::shared_ptr<api::IManager<Any>> &manager, size_t size)
        : data(new api::ICollectionManager<IObject::Any>::Class()), collection_manager(manager->collectionManager()) {
    setManager(manager);
}

std::shared_ptr<iterator::ICoreReadIterator<IObject::Any>> IMemoryObject::readIterator() {
    return std::make_shared<iterator::IReadMemoryIterator>(data, collection_manager);
}

std::shared_ptr<iterator::ICoreWriteIterator<IObject::Any>> IMemoryObject::writeIterator() {
    return std::make_shared<iterator::IWriteMemoryIterator>(data, collection_manager);
}

void IMemoryObject::read(std::shared_ptr<transport::TTransport> trans) {
    auto data_transport = std::make_shared<data::IZlibTransport>(trans);
    data::IObjectProtocol data_proto(data_transport);
    auto reader = collection_manager->reader();
    clear();
    delete data;
    data = data_proto.readPtrObject(*reader);
}

void IMemoryObject::write(std::shared_ptr<transport::TTransport> trans, int8_t compression) {
    auto data_transport = std::make_shared<data::IZlibTransport>(trans, compression);
    data::IObjectProtocol data_proto(data_transport);
    auto writer = collection_manager->writer();
    data_proto.writeObject(*data, *writer);
    data_transport->flush();
}

void IMemoryObject::copyFrom(IObject &source) {
    iterator::readToWrite(*(source.readIterator()), *(this->writeIterator()));
}

void IMemoryObject::moveFrom(IObject &source) {
    iterator::readToWrite(*(source.readIterator()), *(this->writeIterator()), true);
    source.clear();
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
    return TYPE;
}

IMemoryObject::~IMemoryObject() {
    clear();
    delete data;
}

