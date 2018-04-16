
#include "IContext.h"

using namespace ignis::executor::api;

IContext::IContext(int64_t &id) : id(id) {}

std::string &IContext::operator[](const std::string key) {
    return properties[key];
}

std::string &IContext::operator[](const std::string &key) {
    return properties[key];
}

std::string &IContext::operator[](const char *key) {
    return properties[key];
}

std::unordered_map<std::string, std::string> &IContext::getProperties() {
    return properties;
}

int64_t IContext::getId() {
    return id;
}


