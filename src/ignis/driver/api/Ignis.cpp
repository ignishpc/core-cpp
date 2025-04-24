
#include "Ignis.h"
#include <boost/process.hpp>
#include <ghc/filesystem.hpp>
#include <ignis/driver/core/ICallBack.h>
#include <mutex>
#include <stdlib.h>

using namespace ignis::driver::api;
std::mutex mutex;
std::shared_ptr<ignis::driver::core::IClientPool> client_pool;
std::shared_ptr<boost::process::child> backend;
std::shared_ptr<boost::process::ipstream> backend_out;
std::shared_ptr<boost::process::opstream> backend_in;
std::shared_ptr<ignis::driver::core::ICallBack> callback;

std::string search_path(const ghc::filesystem::path &filename) {
    std::string path = std::getenv("PATH");
    std::stringstream ss(path);
    std::string dir;
    while (getline(ss, dir, ':')) {
        auto p = dir / filename;
        std::error_code ec;
        bool file = ghc::filesystem::is_regular_file(p, ec);
        if (!ec && file && ::access(p.c_str(), X_OK) == 0) return p;
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

        const char *str_transport_cmp = std::getenv("IGNIS_TRANSPORT_COMPRESSION");
        int transport_cmp = str_transport_cmp ? std::atoi(str_transport_cmp) : 0;
        const std::string job_sockets = std::getenv("IGNIS_JOB_SOCKETS");

        backend_in = std::make_shared<boost::process::opstream>();
        backend = std::make_shared<boost::process::child>(
                search_path("ignis-backend"), boost::process::std_out > stdout, boost::process::std_err > stderr,
                boost::process::std_in < (*backend_in));

        callback = std::make_shared<ignis::driver::core::ICallBack>(job_sockets + "driver.sock", transport_cmp);
        client_pool = std::make_shared<core::IClientPool>(job_sockets + "backend.sock", transport_cmp);
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
    if (!callback) { throw IDriverException("Ignis.start() must be invoked before the other routines"); }
    return callback->getDriverContext();
}

ignis::driver::core::IClientPool &Ignis::clientPool() {
    if (!client_pool) { throw IDriverException("Ignis.start() must be invoked before the other routines"); }
    return *client_pool;
}