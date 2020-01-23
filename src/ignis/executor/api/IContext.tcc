
#include "IContext.h"

#define IContextClass ignis::executor::api::IContext

template<typename T>
T &IContextClass::var(const std::string &name) {
    auto it = variables.find(name);
    if (it == variables.end()) {
        auto value = std::make_pair(true, std::make_shared<T>());
        variables[name] = std::static_pointer_cast<std::pair<bool, std::shared_ptr<void>> &>(value);
        return value.second;
    } else if (it->second.first) {
        return *std::static_pointer_cast<T*>(it->second.second.get());
    } else {
        auto value = varProtocol(it->second.second)->readObject<std::shared_ptr<T>>();
        it->second.first = true;
        it->second.second = std::static_pointer_cast<std::shared_ptr<void> &>(value);
        return value;
    }
}

#undef IContextClass