
#include "Ignis.h"
#include <boost/process.hpp>
#include <ignis/driver/core/ICallBack.h>
#include <ghc/filesystem.hpp>
#include <mutex>
#include <stdlib.h>

using namespace ignis::driver::api;
std::mutex mutex;
std::shared_ptr<ignis::driver::core::IClientPool> client_pool;
std::shared_ptr<boost::process::child> backend;
std::shared_ptr<boost::process::ipstream> backend_out;
std::shared_ptr<boost::process::opstream> backend_in;
std::shared_ptr<ignis::driver::core::ICallBack> callback;

std::string search_path(const ghc::filesystem::path& filename){
    std::string path = std::getenv("PATH");
    std::stringstream ss(path);
    std::string dir;
    while(getline(ss, dir, ':')){
        auto p = dir / filename;
        std::error_code ec;
        bool file = ghc::filesystem::is_regular_file(p, ec);
        if (!ec && file && ::access(p.c_str(), X_OK) == 0)
            return p;
    }
    return filename;
}

int parseInt(std::shared_ptr<boost::process::ipstream> &in) {
    char buffer[10];
    in->getline(buffer, 10, '\n');
    return std::atoi(buffer);
}

void Ignis::start() {
    try {
        std::lock_guard<std::mutex> lock(mutex);
        if (client_pool) { return; }
        if (!MPI::Is_initialized()) { MPI::Init(); }
        backend_out = std::make_shared<boost::process::ipstream>();
        backend_in = std::make_shared<boost::process::opstream>();
        backend = std::make_shared<boost::process::child>(
                search_path("ignis-backend"), boost::process::std_out > (*backend_out),
                boost::process::std_err > stderr, boost::process::std_in < (*backend_in));
        int backend_port = parseInt(backend_out);
        int backend_compression = parseInt(backend_out);
        int callback_port = parseInt(backend_out);
        int callback_compression = parseInt(backend_out);
        callback = std::make_shared<ignis::driver::core::ICallBack>(callback_port, callback_compression);
        client_pool = std::make_shared<core::IClientPool>(backend_port, backend_compression);
    } catch (executor::core::exception::IException &ex) { throw IDriverException(ex.what(), ex.toString()); }
}

void Ignis::stop() {
    std::lock_guard<std::mutex> lock(mutex);
    if (!client_pool) { return; }
    {
        auto client = client_pool->getClient();
        client->getBackendService().stop();
    }
    backend->wait();
    client_pool.reset();
    backend.reset();
    backend_out.reset();
    backend_in.reset();
    callback.reset();
}

ignis::driver::core::IDriverContext &Ignis::driverContext() {
    if (!callback) {
        throw IDriverException("Ignis.start() must be invoked before the other routines");
    }
    return callback->getDriverContext();
}

ignis::driver::core::IClientPool &Ignis::clientPool() {
    if (!client_pool) {
        throw IDriverException("Ignis.start() must be invoked before the other routines");
    }
    return *client_pool;
}