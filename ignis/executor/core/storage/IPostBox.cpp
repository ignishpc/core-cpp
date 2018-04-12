
#include "IPostBox.h"

using namespace ignis::executor::core::storage;

IPostBox::IPostBox() {}

std::unordered_map<size_t, IMessage> IPostBox::getMessages() {
    std::lock_guard<std::mutex> lock(mutex);
    return std::move(outbox);
}

void IPostBox::newMessage(size_t id, IMessage &msg) {
    std::lock_guard<std::mutex> lock(mutex);
    outbox[id] = msg;
}

std::unordered_map<size_t, IMessage> IPostBox::getOutMessages() {
    std::lock_guard<std::mutex> lock(mutex);
    return std::move(outbox);
}

void IPostBox::newInMessage(size_t id, IMessage &msg) {
    std::lock_guard<std::mutex> lock(mutex);
    inbox[id] = msg;
}