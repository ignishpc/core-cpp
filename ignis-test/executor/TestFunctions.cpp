
#include "executor/api/function/IFunction2.h"

using namespace ignis::executor::api;

class ReduceInt : public function::IFunction2<int, int, int> {
public:
    int call(int &v1, int &v2, IContext &context) override {
        return v1 + v2;
    }
};

ignis_export(ReduceInt, ReduceInt)


class ReduceString : public function::IFunction2<std::string, std::string, std::string> {
public:
    std::string call(std::string &v1, std::string &v2, IContext &context) override {
        return v1 + v2;
    }
};

ignis_export(ReduceString, ReduceString)