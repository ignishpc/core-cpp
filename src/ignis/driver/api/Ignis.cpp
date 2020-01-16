
#include "Ignis.h"
#include <ignis/driver/core/ICallBack.h>
#include <mutex>
#include <boost/process.hpp>
#include <stdlib.h>

using namespace ignis::driver::api;
std::mutex mutex;
std::shared_ptr<ignis::driver::core::IClientPool> Ignis::clientPool;
std::shared_ptr<ignis::driver::core::IDriverContext> Ignis::driverContext;
std::shared_ptr<boost::process::child> backend;
std::shared_ptr<boost::process::ipstream> backend_out;
std::shared_ptr<boost::process::opstream> backend_in;
std::shared_ptr<ignis::driver::core::ICallBack> callback;

int parseInt(std::shared_ptr<boost::process::ipstream>& in){
    char buffer[10];
    in->getline(buffer,10,'\n');
    return std::atoi(buffer);
}

void Ignis::start() {
    std::lock_guard<std::mutex> lock(mutex);
    if (clientPool) {
        return;
    }
    backend_out = std::make_shared<boost::process::ipstream>();
    backend_in = std::make_shared<boost::process::opstream>();
    backend = std::make_shared<boost::process::child>(boost::process::search_path("ignis-backend"),
                                                      boost::process::std_out > (*backend_out),
                                                      boost::process::std_err > stderr,
                                                      boost::process::std_in < (*backend_in));
    int backend_port = parseInt(backend_out);
    int backend_compression = parseInt(backend_out);
    int callback_port = parseInt(backend_out);
    int callback_compression = parseInt(backend_out);
    clientPool = std::make_shared<core::IClientPool>(backend_port, backend_compression);
    callback = std::make_shared<ignis::driver::core::ICallBack>(callback_port, callback_compression);
}

void Ignis::stop() {
    std::lock_guard<std::mutex> lock(mutex);
    if (!clientPool) {
        return;
    }
    {
        auto client = clientPool->getClient();
        client->getBackendService().stop();
    }
    clientPool.reset();
    backend.reset();
    backend_out.reset();
    backend_in.reset();
    callback.reset();
}