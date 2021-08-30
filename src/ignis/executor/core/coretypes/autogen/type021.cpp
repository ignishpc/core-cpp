
#include "ignis/executor/core/selector/ISelector.h"
#include <memory>
#include <string>

using namespace ignis::executor::api;
using namespace ignis::executor::core;


std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type021 = std::make_shared<ignis::executor::core::selector::ITypeSelectorImpl<std::pair<std::string, std::vector<int64_t>>>>();

