
#include "IContext.h"
#include "../core/IObjectLoader.h"

using namespace ignis::executor::api;

std::string &IContext::operator[](const std::string &key) {
    return properties[key];
}

std::unordered_map<std::string, std::string> &IContext::getProperties() {
    return properties;
}

bool IContext::containsVariable(const std::string &name) {
    return variables.find(name) != variables.end();
}

std::unordered_map<std::string, std::pair<bool, std::shared_ptr<void>>> &IContext::getVariables() {
    return variables;
}

std::shared_ptr<void> IContext::decodeVariable(std::shared_ptr<void> var, std::shared_ptr<void> manager) {
    return ((core::IObjectLoader *) loader)->loadVariable(
            (*(std::string *) var.get()),
            std::static_pointer_cast<executor::api::IManager<core::storage::IObject::Any>>(manager));
}

void IContext::vregisterManager(std::shared_ptr<void> manager) {
    ((core::IObjectLoader *) loader)->registermanager(
            std::static_pointer_cast<executor::api::IManager<core::storage::IObject::Any>>(manager));

}

IContext::IContext(void *loader) : loader(loader) {}

IContext::~IContext() {

}

