
#include "IProperties.h"
#include "IDriverException.h"
#include "ignis/rpc/driver/IDriverException_types.h"

using namespace ignis::driver::api;

IProperties::IProperties() {
    try {
        id = Ignis::clientPool().getClient()->getPropertiesService().newInstance();
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex.cause_); }
}

IProperties::IProperties(const IProperties &properties) {
    try {
        id = Ignis::clientPool().getClient()->getPropertiesService().newInstance2(properties.id);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex.cause_); }
}

std::string IProperties::set(const std::string &key, const std::string &value) {
    try {
        std::string _return;
        Ignis::clientPool().getClient()->getPropertiesService().setProperty(_return, id, key, value);
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex.cause_); }
}

std::string IProperties::get(const std::string &key) {
    try {
        std::string _return;
        Ignis::clientPool().getClient()->getPropertiesService().getProperty(_return, id, key);
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex.cause_); }
}

std::string IProperties::rm(const std::string &key) {
    try {
        std::string _return;
        Ignis::clientPool().getClient()->getPropertiesService().rmProperty(_return, id, key);
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex.cause_); }
}

IProperties::Value IProperties::operator[](const std::string &key) { return Value(*this, key, get(key)); }

std::string IProperties::Value::operator=(const std::string &value) { return properties.set(key, value); }

IProperties::Value::Value(IProperties &properties, const std::string &key, const std::string &value)
    : properties(properties), key(key), value(value) {}

bool IProperties::contains(const std::string &key) {
    try {
        return Ignis::clientPool().getClient()->getPropertiesService().contains(id, key);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex.cause_); }
}

std::map<std::string, std::string> IProperties::toMap(bool defaults) {
    try {
        std::map<std::string, std::string> _return;
        Ignis::clientPool().getClient()->getPropertiesService().toMap(_return, id, defaults);
        return _return;
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex.cause_); }
}

void IProperties::fromMap(const std::map<std::string, std::string> &map) {
    try {
        Ignis::clientPool().getClient()->getPropertiesService().fromMap(id, map);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex.cause_); }
}

void IProperties::load(const std::string &path) {
    try {
        Ignis::clientPool().getClient()->getPropertiesService().load(id, path);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex.cause_); }
}

void IProperties::store(const std::string &path) {
    try {
        Ignis::clientPool().getClient()->getPropertiesService().store(id, path);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex.cause_); }
}

void IProperties::clear() {
    try {
        Ignis::clientPool().getClient()->getPropertiesService().clear(id);
    } catch (rpc::driver::IDriverException &ex) { throw IDriverException(ex.message, ex.cause_); }
}
