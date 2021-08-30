
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


struct sjoin : public function::IFunction2<std::string, std::string, std::string> {
    std::string delimiter;

    void before(IContext &context) {
        if (context.isVar("delimiter")) {
            delimiter = context.var<std::string>("delimiter");
        } else {
            delimiter = "";
        }
    }

    std::string call(std::string &v1, std::string &v2, IContext &context) { return v1 + delimiter + v2; }
};


std::shared_ptr<ignis::executor::core::selector::ISelectorGroup> function005 = 
  std::make_shared<ignis::executor::core::selector::ISelectorGroupImpl<sjoin>>("sjoin");
