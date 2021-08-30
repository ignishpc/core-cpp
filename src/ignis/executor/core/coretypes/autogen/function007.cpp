
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


struct isum : public function::IFunction2<int64_t, int64_t, int64_t> {
    int64_t call(int64_t &v1, int64_t &v2, IContext &context) { return v1 + v2; }
};

std::shared_ptr<ignis::executor::core::selector::ISelectorGroup> function007 = 
  std::make_shared<ignis::executor::core::selector::ISelectorGroupImpl<isum>>("isum");
