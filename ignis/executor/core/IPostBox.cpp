
#include "IPostBox.h"

using namespace ignis::executor::core;

IPostBox::IPostBox() {}

std::unordered_map<size_t, IMessage>IPostBox::getInBox() {
    return inbox;
}

std::unordered_map<size_t, IMessage>IPostBox::popInBox() {
    return std::move(inbox);
}

std::unordered_map<size_t, IMessage> IPostBox::getOutBox() {
    return outbox;
}

std::unordered_map<size_t, IMessage> IPostBox::popOutBox() {
    return std::move(outbox);
}

void IPostBox::newInMessage(size_t id, const IMessage &msg) {
    inbox[id] = msg;
}

void IPostBox::newOutMessage(size_t id, const IMessage &msg) {
    outbox[id] = msg;
}

void IPostBox::clearInBox() {
    inbox.clear();
}

void IPostBox::clearOutBox() {
    outbox.clear();
}

IPostBox::~IPostBox() {

}
