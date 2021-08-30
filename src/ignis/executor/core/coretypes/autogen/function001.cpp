
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


struct stoi : public function::IFunction<std::string, int64_t> {

    int64_t call(std::string &v, IContext &context) { return std::stoi(v); }
};


std::shared_ptr<ignis::executor::core::selector::ISelectorGroup> function001 = 
  std::make_shared<ignis::executor::core::selector::ISelectorGroupImpl<stoi>>("stoi");
