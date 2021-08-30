
#include "ignis/executor/core/selector/ISelector.h"
#include <memory>
#include <string>

using namespace ignis::executor::api;
using namespace ignis::executor::core;


std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type028 = std::make_shared<ignis::executor::core::selector::ITypeSelectorImpl<std::pair<double, std::pair<double, double>>>>();

