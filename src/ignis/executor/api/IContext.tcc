
#include "IContext.h"

#define IContextClass ignis::executor::api::IContext

template<typename Tp>
Tp &IContextClass::var(const std::string &name) {
    auto it = variables.find(name);
    if (it == variables.end()) {
        auto value = std::make_shared<executor::core::IBasicVariable<Tp>>();
        variables[name] = value;
        return value->get();
    } else if (it->second->decoded()) {
        return it->second->basic<Tp>().get();
    } else {
        it->second = it->second->bytes().decode<Tp>();
        return it->second->basic<Tp>().get();
    }
}

#undef IContextClass