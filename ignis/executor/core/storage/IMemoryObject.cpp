
#include "IMemoryObject.h"
#include <thrift/transport/TZlibTransport.h>
#include "../../../data/IObjectProtocol.h"
#include "IMemoryIterator.h"

using namespace std;
using namespace ignis::executor::core::storage;
using namespace ignis::data::serialization;
using namespace apache::thrift::transport;

IMemoryObject::IMemoryObject(const std::shared_ptr<data::IManager<Any>> &manager, size_t size)
        : manager(manager),
          data(new std::vector<IObject::Any>()),
          handle(make_shared<IObject::Handle<vector<IObject::Any>>>(data,
                                                                    manager->getClassManagerType()->getTypeHandle())) {}

shared_ptr<ICoreReadIterator<IObject::Any>> IMemoryObject::readIterator() {
    return make_shared<IReadMemoryIterator>(data, manager);
}

shared_ptr<ICoreWriteIterator<IObject::Any>> IMemoryObject::writeIterator() {
    return make_shared<IWriteMemoryIterator>(data, manager);
}

void IMemoryObject::read(std::shared_ptr<apache::thrift::transport::TTransport> trans) {
    auto data_transport = make_shared<TZlibTransport>(trans);
    data::IObjectProtocol data_proto(data_transport);
    auto type_handle = manager->getClassManagerType()->getTypeHandle();
    auto reader = type_handle->reader();
    data = reader->readPtr(data_proto);
    handle = make_shared<IObject::Handle <vector<IObject::Any>> > (data, type_handle);
}

void IMemoryObject::write(std::shared_ptr<apache::thrift::transport::TTransport> trans, int8_t compression) {
    auto data_transport = make_shared<TZlibTransport>(trans, 128, 1024, 128, 1024, compression);
    data::IObjectProtocol data_proto(data_transport);
    auto writer = manager->getClassManagerType()->getTypeHandle()->writer();
    writer->writePtr(data, data_proto);
}

IMemoryObject::~IMemoryObject() {

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

string IMemoryObject::getType() {
    return "memory";
}


