
#include "ignis/executor/api/function/IBeforeFunction.h"
#include "ignis/executor/api/function/IFunction.h"
#include "ignis/executor/api/function/IFunction0.h"
#include "ignis/executor/api/function/IFunction2.h"
#include "ignis/executor/api/function/IVoidFunction.h"
#include "ignis/executor/api/function/IVoidFunction0.h"
#include "ignis/executor/api/function/IVoidFunction2.h"
#include <memory>
#include <string>

using namespace ignis::executor::api;
using namespace ignis::executor::core;


struct dtos : public function::IFunction<double, std::string> {

    std::string call(double &v, IContext &context) { return std::to_string(v); }
};


std::shared_ptr<ignis::executor::core::selector::ISelectorGroup> function002 = 
  std::make_shared<ignis::executor::core::selector::ISelectorGroupImpl<dtos>>("dtos");
