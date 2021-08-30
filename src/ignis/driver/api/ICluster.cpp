
#include "ICluster.h"
#include "IDriverException.h"
#include "ignis/rpc/driver/IDriverException_types.h"

using namespace ignis::driver::api;

ICluster::ICluster() {
    try {
        id = Ignis::clientPool().getClient()->getClusterService().newInstance0();
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex.cause_); }
}

ICluster::ICluster(const std::string &name) {
    try {
        id = Ignis::clientPool().getClient()->getClusterService().newInstance1a(name);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex.cause_); }
}

ICluster::ICluster(const IProperties &properties) {
    try {
        id = Ignis::clientPool().getClient()->getClusterService().newInstance1b(properties.id);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex.cause_); }
}

ICluster::ICluster(const std::string &name, const IProperties &properties) {
    try {
        id = Ignis::clientPool().getClient()->getClusterService().newInstance2(name, properties.id);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex.cause_); }
}

void ICluster::start(){
    try {
        Ignis::clientPool().getClient()->getClusterService().start(id);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex.cause_); }
}

void ICluster::destroy(){
    try {
        Ignis::clientPool().getClient()->getClusterService().destroy(id);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex.cause_); }
}

void ICluster::setName(const std::string &name) {
    try {
        Ignis::clientPool().getClient()->getClusterService().setName(id, name);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex.cause_); }
}

void ICluster::execute(const std::vector<std::string> &cmd) {
    try {
        Ignis::clientPool().getClient()->getClusterService().execute(id, cmd);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex.cause_); }
}

void ICluster::execute(const std::string &cmd...) {
    try {
        execute({cmd});
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex.cause_); }
}

void ICluster::execute(const std::initializer_list<std::string> &cmd) {
    std::vector<std::string> v;
    for (auto &value : cmd) { v.push_back(value); }
    execute(cmd);
}

void ICluster::executeScript(const std::string &script) {
    try {
        Ignis::clientPool().getClient()->getClusterService().executeScript(id, script);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex.cause_); }
}

void ICluster::sendFile(const std::string &source, const std::string &target) {
    try {
        Ignis::clientPool().getClient()->getClusterService().sendFile(id, source, target);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex.cause_); }
}

void ICluster::sendCompressedFile(const std::string &source, const std::string &target) {
    try {
        Ignis::clientPool().getClient()->getClusterService().sendCompressedFile(id, source, target);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex.cause_); }
}
