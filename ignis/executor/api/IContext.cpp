
#include "IContext.h"

using namespace ignis::executor::api;

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

