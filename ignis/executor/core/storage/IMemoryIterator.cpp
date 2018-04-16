
#include "IMemoryIterator.h"

using namespace std;
using namespace ignis::executor::core::storage;
using namespace ignis::data::serialization;
using namespace apache::thrift::transport;


IObject::Any &IReadMemoryIterator::next() {
    return data_handle->get(*data, pos++);
}

bool IReadMemoryIterator::hashNext() {
    return pos < elems;
}

IReadMemoryIterator::~IReadMemoryIterator() {
}

IReadMemoryIterator::IReadMemoryIterator(std::vector<IObject::Any> *data,
                                         const shared_ptr<IColectionHandle<IObject::Any>> &data_handle)
        : data(data), data_handle(data_handle) {}


void IWriteMemoryIterator::write(IObject::Any &obj) {
    data_handle->push(*data, obj);
}

void IWriteMemoryIterator::write(IObject::Any &&obj) {
    data_handle->push(*data, obj);
}

IWriteMemoryIterator::~IWriteMemoryIterator() {

}

IWriteMemoryIterator::IWriteMemoryIterator(std::vector<IObject::Any> *data,
                                           const shared_ptr<IColectionHandle<IObject::Any>> &data_handle)
        : data(data), data_handle(data_handle) {}


