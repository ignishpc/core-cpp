
#include "ignis/executor/core/ICoreTypes.h"




std::vector<std::shared_ptr<ignis::executor::core::selector::ITypeSelector>> ignis::executor::core::ICoreTypes::defaultTypes() {
    return {};
}

std::vector<std::shared_ptr<ignis::executor::core::selector::ISelectorGroup>> ignis::executor::core::ICoreTypes::defaultFunctions() {
    return {};
}
