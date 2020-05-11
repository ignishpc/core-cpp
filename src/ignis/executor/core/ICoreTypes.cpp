
#include "ICoreTypes.h"

#include <memory>
#include <string>

using namespace ignis::executor::core;
using ignis::executor::core::selector::ITypeSelectorImpl;

std::vector<std::shared_ptr<selector::ITypeSelector>> ICoreTypes::defaultTypes() {
    return {
            std::make_shared<ITypeSelectorImpl<bool>>(),
            std::make_shared<ITypeSelectorImpl<int8_t>>(),
            std::make_shared<ITypeSelectorImpl<int16_t>>(),
            std::make_shared<ITypeSelectorImpl<int32_t>>(),
            std::make_shared<ITypeSelectorImpl<int64_t>>(),
            std::make_shared<ITypeSelectorImpl<double>>(),
            std::make_shared<ITypeSelectorImpl<std::string>>(),
            std::make_shared<ITypeSelectorImpl<std::pair<std::string, bool>>>(),
            std::make_shared<ITypeSelectorImpl<std::pair<std::string, int32_t>>>(),
            std::make_shared<ITypeSelectorImpl<std::pair<std::string, int64_t>>>(),
            std::make_shared<ITypeSelectorImpl<std::pair<std::string, double>>>(),
            std::make_shared<ITypeSelectorImpl<std::pair<int64_t, bool>>>(),
            std::make_shared<ITypeSelectorImpl<std::pair<int64_t, int64_t>>>(),
            std::make_shared<ITypeSelectorImpl<std::pair<int64_t, std::string>>>()
    };
}

