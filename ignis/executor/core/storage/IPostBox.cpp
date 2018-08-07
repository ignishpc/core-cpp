
#include "IPostBox.h"

using namespace ignis::executor::core::storage;

IPostBox::IPostBox() {}

std::vector<IMessage> IPostBox::getMessages() {
    std::lock_guard<std::mutex> lock(mutex);
    return std::move(outbox);
}

void IPostBox::newMessage(IMessage &msg) {
    std::lock_guard<std::mutex> lock(mutex);
    outbox.push_back(msg);
}

std::vector<IMessage> IPostBox::getOutMessages() {
    std::lock_guard<std::mutex> lock(mutex);
    return std::move(outbox);
}

void IPostBox::newInMessage(IMessage &msg) {
    std::lock_guard<std::mutex> lock(mutex);
    inbox.push_back(msg);
}

IPostBox::~IPostBox() {

}
