
#include "IElementIterator.h"

using namespace ignis::executor::core;
using namespace ignis::executor::api;
using namespace ignis::executor::core::storage;

storage::iterator::IElementIterator::IElementIterator(const std::shared_ptr<api::IManager<storage::IObject::Any>> &manager)
        : data(manager->_new(),manager) {}

storage::IObject::Any &storage::iterator::IElementIterator::next() {
    return data.get();
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
    data.getManager()->copy(obj,data.get());
}

void storage::iterator::IElementIterator::write(storage::IObject::Any &&obj) {
    data.getManager()->move(obj,data.get());
}

storage::iterator::IElementIterator::~IElementIterator() {
}
