#include "../../../../ignis/executor/api/function/IFunction.h"
#include "../../../../ignis/executor/api/function/IFlatFunction.h"
#include "../../../../ignis/executor/api/function/IFunction2.h"
#include <iostream>

namespace function = ignis::executor::api::function;

class mapFunction : public function::IFunction<int, std::string> {
public:

    std::string call(int &t, ignis::executor::api::IContext &context) override {
        return std::to_string(t);
    }

};

ignis_register_class(mapFunction, mapFunction);

class flatmapFunction : public function::IFlatFunction<int, std::string> {
public:

    ignis::executor::api::Iterable<std::string> call(int &t, ignis::executor::api::IContext &context) override {
        std::vector<std::string> v;
        if (t % 2 == 0) {
            v.push_back(std::to_string(t));
        }
        return ignis::executor::api::Iterable<std::string>::fromCollection(v);
    }

};

ignis_register_class(flatmapFunction, flatmapFunction);


class filterFunction : public function::IFunction<int, bool> {
public:

    bool call(int &t, ignis::executor::api::IContext &context) override {
        return t % 2 == 0;
    }

};

ignis_register_class(filterFunction, filterFunction);

class reduceByKeyFunction : public function::IFunction2<int, int, int> {
public:

    int call(int &t1, int &t2, ignis::executor::api::IContext &context) override {
        return t1 + t2;
    }

};

ignis_register_class(reduceByKeyFunction, reduceByKeyFunction);

