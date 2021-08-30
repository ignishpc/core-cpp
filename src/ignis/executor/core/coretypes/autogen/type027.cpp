
#include "ignis/executor/core/selector/ISelector.h"
#include <memory>
#include <string>

using namespace ignis::executor::api;
using namespace ignis::executor::core;


std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type027 = std::make_shared<ignis::executor::core::selector::ITypeSelectorImpl<std::pair<double, std::pair<int64_t, int64_t>>>>();

