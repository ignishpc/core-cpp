
#include "IClientPool.h"

using namespace ignis::driver::core;

IClientPool::IClientPool(const std::string& usock, int compression) : usock(usock), compression(compression) {}

std::shared_ptr<IClient> IClientPool::getClient() {
    std::shared_ptr<IClient> client;
    mutex.lock();
    if (!queue.empty()) {
        client = queue.front();
        queue.pop_front();
    }
    mutex.unlock();
    if (!client) {
        client = std::make_shared<IClient>(usock, compression);
        mutex.lock();
        clients.push_back(client);
        mutex.unlock();
    }
    auto release = [this, client](IClient *) {
        mutex.lock();
        queue.push_back(client);
        mutex.unlock();
    };
    return std::shared_ptr<IClient>(client.get(), release);
}