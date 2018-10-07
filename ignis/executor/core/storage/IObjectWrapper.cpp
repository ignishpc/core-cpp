#include "IObjectWrapper.h"

using namespace ignis::executor::core::storage;

IObjectWrapper::IObjectWrapper(const std::shared_ptr<IObject> &to_read, const std::shared_ptr<IObject> &to_write)
        : to_read(to_read), to_write(to_write) {}

class IReadWrapperIterator : public iterator::ICoreReadIterator<IObject::Any>{
public:
    IReadWrapperIterator(const std::shared_ptr<iterator::ICoreReadIterator<IObject::Any>> &to_read,
                         const std::shared_ptr<iterator::ICoreWriteIterator<IObject::Any>> &to_write) : to_read(to_read),
                                                                                             to_write(to_write) {}

    IObject::Any &next() override {
        IObject::Any &value = to_read->next();
        to_write->write(value);
        return value;
    }

    bool hashNext() override {
        return to_read->hashNext();
    }

    bool isMoved() override {
        return to_read->isMoved();
    }

    virtual ~IReadWrapperIterator() override {
    }

private:
    std::shared_ptr<iterator::ICoreReadIterator<IObject::Any>> to_read;
    std::shared_ptr<iterator::ICoreWriteIterator<IObject::Any>> to_write;
};

std::shared_ptr<iterator::ICoreReadIterator<IObject::Any>> IObjectWrapper::readIterator() {
    return std::make_shared<IReadWrapperIterator>(to_read->readIterator(), to_write->writeIterator());
}

std::shared_ptr<iterator::ICoreWriteIterator<IObject::Any>> IObjectWrapper::writeIterator() {
    return to_read->writeIterator();
}

void IObjectWrapper::read(std::shared_ptr<transport::TTransport> trans) {
    to_read->read(trans);
}

void IObjectWrapper::write(std::shared_ptr<transport::TTransport> trans, int8_t compression) {
    to_read->write(trans,compression);
}

void IObjectWrapper::copyFrom(IObject &source) {
    iterator::readToWrite(*(source.readIterator()), *(this->writeIterator()));
}

void IObjectWrapper::moveFrom(IObject &source) {
    iterator::readToWrite(*(source.readIterator()), *(this->writeIterator()), true);
}

size_t IObjectWrapper::getSize() {
    return to_read->getSize();
}

bool IObjectWrapper::hasSize() {
    return to_read->hasSize();
}

void IObjectWrapper::clear() {
    to_read->clear();
}

void IObjectWrapper::fit() {
    to_read->fit();
}

std::string IObjectWrapper::getType() {
    return to_read->getType();
}

std::shared_ptr<ignis::executor::api::IManager<IObject::Any>> IObjectWrapper::getManager() {
    return to_read->getManager();
}

void IObjectWrapper::setManager(std::shared_ptr<api::IManager<IObject::Any>> manager) {
    to_read->setManager(manager);
}

IObjectWrapper::~IObjectWrapper() {

}


