
#include "ICoreTypes.h"

#include <memory>
#include <string>

using namespace ignis::executor::core;
using ignis::executor::core::selector::ITypeSelectorImpl;

std::vector<std::shared_ptr<selector::ITypeSelector>> ICoreTypes::defaultTypes() {
    return {
            std::make_shared<ITypeSelectorImpl<bool>>(),
            std::make_shared<ITypeSelectorImpl<int32_t>>(),
            std::make_shared<ITypeSelectorImpl<int64_t>>(),
            std::make_shared<ITypeSelectorImpl<double>>(),
            std::make_shared<ITypeSelectorImpl<std::string>>(),
            std::make_shared<ITypeSelectorImpl<std::pair<std::string, bool>>>(),
            std::make_shared<ITypeSelectorImpl<std::pair<std::string, int32_t>>>(),
            std::make_shared<ITypeSelectorImpl<std::pair<std::string, int64_t>>>(),
            std::make_shared<ITypeSelectorImpl<std::pair<std::string, double>>>(),
            std::make_shared<ITypeSelectorImpl<std::pair<std::string, std::string>>>(),
            std::make_shared<ITypeSelectorImpl<std::pair<double, bool>>>(),
            std::make_shared<ITypeSelectorImpl<std::pair<double, double>>>(),
            std::make_shared<ITypeSelectorImpl<std::pair<double, std::string>>>(),
            /*GroupBy key defaults*/
            std::make_shared<ITypeSelectorImpl<std::pair<std::string, std::vector<double>>>>(),
            std::make_shared<ITypeSelectorImpl<std::pair<std::string, std::vector<std::string>>>>(),
            std::make_shared<ITypeSelectorImpl<std::pair<double, std::vector<double>>>>(),
            std::make_shared<ITypeSelectorImpl<std::pair<double, std::vector<std::string>>>>(),
            /*Join defaults*/
            std::make_shared<ITypeSelectorImpl<std::pair<std::string, std::pair<double, double>>>>(),
            std::make_shared<ITypeSelectorImpl<std::pair<std::string, std::pair<std::string, std::string>>>>(),
            std::make_shared<ITypeSelectorImpl<std::pair<double, std::pair<double, double>>>>(),
            std::make_shared<ITypeSelectorImpl<std::pair<double, std::pair<std::string, std::string>>>>(),
    };
}

std::vector<std::shared_ptr<selector::ISelectorGroup>> ICoreTypes::defaultFunctions() { return {}; }
