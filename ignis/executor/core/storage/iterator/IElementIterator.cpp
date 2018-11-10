
#include "IElementIterator.h"

using namespace ignis::executor::core;
using namespace ignis::executor::api;
using namespace ignis::executor::core::storage;

iterator::IElementIterator::IElementIterator(const std::shared_ptr<api::IManager<IObject::Any>> &manager) :
        manager(manager), data(storage::IObject::toShared(manager->_new(), manager)) {

}

iterator::IElementIterator::IElementIterator(
        const std::shared_ptr<ignis::executor::api::IManager<IObject::Any>> &manager,
        const std::shared_ptr<IObject::Any> &data) : manager(manager), data(data) {

}

storage::IObject::Any &storage::iterator::IElementIterator::next() {
    return *data;
}

std::shared_ptr<storage::IObject::Any> storage::iterator::IElementIterator::nextShared() {
    return data;
}

bool storage::iterator::IElementIterator::hasNext() {
    return true;
}

bool storage::iterator::IElementIterator::isMoved() {
    return true;
}

void storage::iterator::IElementIterator::skip(size_t elems) {
}

void storage::iterator::IElementIterator::write(storage::IObject::Any &obj) {
    manager->copy(obj, *data);
}

void storage::iterator::IElementIterator::write(storage::IObject::Any &&obj) {
    manager->move(obj, *data);
}

storage::iterator::IElementIterator::~IElementIterator() {
}
