
#include "ignis/executor/core/selector/ISelector.h"
#include <memory>
#include <string>

using namespace ignis::executor::api;
using namespace ignis::executor::core;


std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type012 = std::make_shared<ignis::executor::core::selector::ITypeSelectorImpl<std::pair<std::string, int64_t>>>();

