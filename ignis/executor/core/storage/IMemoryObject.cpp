
#include "IMemoryObject.h"
#include <thrift/transport/TZlibTransport.h>
#include "../../../data/IObjectProtocol.h"
#include "IMemoryIterator.h"

using namespace std;
using namespace ignis::executor::core::storage;
using namespace ignis::data::serialization;
using namespace apache::thrift::transport;


IMemoryObject::IMemoryObject(const shared_ptr<ITypeHandleBase<Any>> &type_handle, uint32_t sz)
        : data(new std::vector<IObject::Any>()),
          data_handle(((ITypeHandle<IObject::Any> &) *type_handle).collectionHandle()),
          handle(make_shared<Handle<vector<IObject::Any>> >(data, data_handle)) {
    data_handle->resize(*data, sz);
    setTypeHandle(type_handle);
}

shared_ptr<ignis::executor::api::IReadIterator<IObject::Any>> IMemoryObject::readIterator() {
    return make_shared<IReadMemoryIterator>(data, data_handle);
}

shared_ptr<ignis::executor::api::IWriteIterator<IObject::Any>> IMemoryObject::writeIterator() {
    return make_shared<IWriteMemoryIterator>(data, data_handle);
}

void IMemoryObject::read(std::shared_ptr<apache::thrift::transport::TTransport> trans) {
    auto data_transport = make_shared<TZlibTransport>(trans);
    data::IObjectProtocol data_proto(data_transport);
    auto reader = data_handle->reader();
    data = reader->readPtr(data_proto);
    handle = make_shared<Handle<vector<IObject::Any>> >(data, data_handle);
}

void IMemoryObject::write(std::shared_ptr<apache::thrift::transport::TTransport> trans, int8_t compression) {
    auto data_transport = make_shared<TZlibTransport>(trans, 128, 1024, 128, 1024, compression);
    data::IObjectProtocol data_proto(data_transport);
    auto writer = data_handle->writer();
    writer->writePtr(data, data_proto);
}

IMemoryObject::~IMemoryObject() {

}

size_t IMemoryObject::getSize() {
    return data_handle->getSize(*data);
}


