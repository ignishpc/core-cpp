
#include "ignis/executor/core/selector/ISelector.h"
#include <memory>
#include <string>

using namespace ignis::executor::api;
using namespace ignis::executor::core;


std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type002 = std::make_shared<ignis::executor::core::selector::ITypeSelectorImpl<int32_t>>();

