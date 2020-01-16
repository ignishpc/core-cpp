
#include "IClientPool.h"

using namespace ignis::driver::core;

IClientPool::IClientPool(int port, int compression) : port(port), compression(compression) {}

std::shared_ptr<IClient> IClientPool::getClient() {
    std::shared_ptr<IClient> client;
    mutex.lock();
    if (!queue.empty()) {
        client = queue.front();
        queue.pop_front();
    }
    mutex.unlock();
    if (!client) {
        client = std::make_shared<IClient>(port, compression);
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