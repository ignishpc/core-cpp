

#include "IObjectWrapperIterator.h"
#include "../../../exceptions/ILogicError.h"
#include "../../../data/IObjectProtocol.h"
#include <thrift/transport/TZlibTransport.h>

using namespace ignis::executor::core::storage;

IObjectWrapperIterator::IObjectWrapperIterator(const std::shared_ptr<ICoreReadIterator<IObject::Any>> &it, size_t size)
        :it(it),size(size) {}


std::shared_ptr<ICoreReadIterator<IObject::Any>> IObjectWrapperIterator::readIterator() {
    return it;
}

std::shared_ptr<ICoreWriteIterator<IObject::Any>> IObjectWrapperIterator::writeIterator() {
    throw exceptions::ILogicError("Read only IObject");
}

void IObjectWrapperIterator::read(std::shared_ptr<apache::thrift::transport::TTransport> trans) {
    throw exceptions::ILogicError("Read only IObject");
}

void IObjectWrapperIterator::write(std::shared_ptr<apache::thrift::transport::TTransport> trans, int8_t compression) {
    auto data_transport = std::make_shared<apache::thrift::transport::TZlibTransport>(trans, 128, 1024, 128, 1024, compression);
    data::IObjectProtocol data_proto(data_transport);
    manager->getClassManagerType()->getTypeHandle()->writer()->writeType(data_proto);
    auto elem_writer = manager->getClassManagerType()->getElemClassManager()->getTypeHandle()->writer();
    data::serialization::writeSizeAux(data_proto,size);
    elem_writer->writeType(data_proto);
    while(it->hashNext()){
        auto& value = it->next();
        elem_writer->writePtr(&value,data_proto);
    }
}

size_t IObjectWrapperIterator::getSize() {
    return size;
}

void IObjectWrapperIterator::setSize(size_t size){
    this->size = size;
}

IObjectWrapperIterator::~IObjectWrapperIterator() {

}




