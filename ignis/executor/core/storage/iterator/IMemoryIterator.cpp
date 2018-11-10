
#include "IMemoryIterator.h"

using namespace ignis::executor::core::storage;
using namespace ignis::executor::core::storage::iterator;


IReadMemoryIterator::IReadMemoryIterator(api::ICollectionManager<IObject::Any>::Class *data,
                                         const std::shared_ptr<api::ICollectionManager<IObject::Any>> &collection_manager)
        : data(data), collection_manager(collection_manager), pos(0), size(collection_manager->size(*data)) {}

IObject::Any &IReadMemoryIterator::next() {
    return collection_manager->get(*data, pos++);
}

std::shared_ptr<IObject::Any> IReadMemoryIterator::nextShared() {
    return std::shared_ptr<IObject::Any>(&next(), [](IObject::Any *elem) {});
}

bool IReadMemoryIterator::hasNext() {
    return pos < size || pos < collection_manager->size(*data);
}

IReadMemoryIterator::~IReadMemoryIterator() {
}

bool IReadMemoryIterator::isMoved() {
    return false;
}

void IReadMemoryIterator::skip(size_t elems) {
    pos += elems;
}

IWriteMemoryIterator::IWriteMemoryIterator(api::ICollectionManager<IObject::Any>::Class *data,
                                           const std::shared_ptr<api::ICollectionManager<IObject::Any>> &collection_manager)
        : data(data), collection_manager(collection_manager) {}


void IWriteMemoryIterator::write(IObject::Any &obj) {
    collection_manager->push(*data, obj);
}

void IWriteMemoryIterator::write(IObject::Any &&obj) {
    collection_manager->push(*data, (IObject::Any &&) obj);
}

IWriteMemoryIterator::~IWriteMemoryIterator() {
}




