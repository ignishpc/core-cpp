

#include "IObjectWrapperIterator.h"
#include "../../../exceptions/ILogicError.h"
#include "../../../data/IObjectProtocol.h"
#include "../../../data/IZlibTransport.h"

using namespace ignis::executor::core::storage;

IObjectWrapperIterator::IObjectWrapperIterator(const std::shared_ptr<iterator::ICoreReadIterator<IObject::Any>> &it, size_t size)
        :it(it),size(size) {}


std::shared_ptr<iterator::ICoreReadIterator<IObject::Any>> IObjectWrapperIterator::readIterator() {
    return it;
}

std::shared_ptr<iterator::ICoreWriteIterator<IObject::Any>> IObjectWrapperIterator::writeIterator() {
    throw exceptions::ILogicError("Read only IObject");
}

void IObjectWrapperIterator::read(std::shared_ptr<transport::TTransport> trans) {
    throw exceptions::ILogicError("Read only IObject");
}

void IObjectWrapperIterator::write(std::shared_ptr<transport::TTransport> trans, int8_t compression) {
    auto data_transport = std::make_shared<data::IZlibTransport>(trans, compression);
    data::IObjectProtocol data_proto(data_transport);
    manager->collectionManager()->writer()->writeType(data_proto);
    auto elem_writer = manager->writer();
    data::handle::writeSizeAux(data_proto,size);
    elem_writer->writeType(data_proto);
    while(it->hashNext()){
        auto& value = it->next();
        elem_writer->writePtr(&value,data_proto);
    }
}

void IObjectWrapperIterator::copyFrom(IObject &source) {
    throw exceptions::ILogicError("Read only IObject");
}

void IObjectWrapperIterator::copyTo(IObject &source) {
    iterator::readToWrite(*(source.readIterator()), *(this->writeIterator()));
}

void IObjectWrapperIterator::moveFrom(IObject &source) {
    throw exceptions::ILogicError("Read only IObject");
}

void IObjectWrapperIterator::moveTo(IObject &source) {
    iterator::readToWrite(*(source.readIterator()), *(this->writeIterator()), true);
}

size_t IObjectWrapperIterator::getSize() {
    return size;
}

void IObjectWrapperIterator::setSize(size_t size){
    this->size = size;
}

std::string IObjectWrapperIterator::getType(){
    return "wrapper iterator";
}

IObjectWrapperIterator::~IObjectWrapperIterator() {

}




