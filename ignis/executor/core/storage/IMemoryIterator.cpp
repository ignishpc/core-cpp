
#include "IMemoryIterator.h"

using namespace std;
using namespace ignis::executor::core::storage;
using namespace ignis::data;
using namespace apache::thrift::transport;


IReadMemoryIterator::IReadMemoryIterator(vector<IObject::Any> *data, const shared_ptr<IManager<IObject::Any>> &manager)
        : data(data), class_manager(manager->getClassManagerType()) {}

IObject::Any &IReadMemoryIterator::next() {
    return class_manager->get(*data, pos++);
}

bool IReadMemoryIterator::hashNext() {
    return pos < elems;
}

IReadMemoryIterator::~IReadMemoryIterator() {
}

IWriteMemoryIterator::IWriteMemoryIterator(vector<IObject::Any> *data, const shared_ptr<IManager<IObject::Any>> &manager)
        : data(data), class_manager(manager->getClassManagerType()) {}


void IWriteMemoryIterator::write(IObject::Any &obj) {
    class_manager->push(*data, obj);
}

void IWriteMemoryIterator::write(IObject::Any &&obj) {
    class_manager->push(*data, obj);
}

IWriteMemoryIterator::~IWriteMemoryIterator() {
}




