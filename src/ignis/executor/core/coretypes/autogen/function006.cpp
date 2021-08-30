
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


struct dsum : public function::IFunction2<double, double, double> {
    double call(double &v1, double &v2, IContext &context) { return v1 + v2; }
};


std::shared_ptr<ignis::executor::core::selector::ISelectorGroup> function006 = 
  std::make_shared<ignis::executor::core::selector::ISelectorGroupImpl<dsum>>("dsum");
