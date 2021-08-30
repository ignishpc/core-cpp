
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


struct ssplit : public function::IFunction<std::string, IVector<std::string>> {
    char delimiter;

    void before(IContext &context) {
        if (context.isVar("delimiter")) {
            delimiter = context.var<std::string>("delimiter").at(0);
        } else {
            delimiter = ',';
        }
    }

    IVector<std::string> call(std::string &v, IContext &context) {
        IVector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(v);
        while (std::getline(tokenStream, token, delimiter)) { tokens.push_back(token); }
        return tokens;
    }
};


std::shared_ptr<ignis::executor::core::selector::ISelectorGroup> function004 = 
  std::make_shared<ignis::executor::core::selector::ISelectorGroupImpl<ssplit>>("ssplit");
