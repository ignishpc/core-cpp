
#include "ignis/executor/core/selector/ISelector.h"
#include <memory>
#include <string>

using namespace ignis::executor::api;
using namespace ignis::executor::core;


std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type009 = std::make_shared<ignis::executor::core::selector::ITypeSelectorImpl<std::pair<double, int64_t>>>();

