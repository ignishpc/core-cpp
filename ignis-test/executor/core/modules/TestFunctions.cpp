#include "../../../../ignis/executor/api/function/IFunction.h"
#include <iostream>

namespace function = ignis::executor::api::function;

class mapFunction : public function::IFunction<int,std::string>{
public:

    void before(ignis::executor::api::IContext &context) override {
        std::cout << "prueba" << std::endl;
    }
};

ignis_register_class(mapFunction,mapFunction);

