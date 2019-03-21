#include "../../../../ignis/executor/api/function/IFunction.h"
#include "../../../../ignis/executor/api/function/IFlatFunction.h"
#include "../../../../ignis/executor/api/function/IFunction2.h"
#include "../../../../ignis/executor/api/IReadIterator.h"
#include <iostream>

namespace api = ignis::executor::api;
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

class mapPartitionFunction :
        public function::IFunction<std::shared_ptr<api::IReadIterator<int>>, std::shared_ptr<api::IReadIterator<int>>> {

    std::shared_ptr<api::IReadIterator<int>>
    call(std::shared_ptr<api::IReadIterator<int>> &t, ignis::executor::api::IContext &context) override {
        int sum = 0;
        while (t->hasNext()) {
            sum += t->next();
        }
        return ignis::executor::api::Iterable<int>::fromValue(sum).readIterator();
    }

};

ignis_register_class(mapPartitionFunction, mapPartitionFunction);

class mapPartitionFunctionWithIndex :
        public function::IFunction2<int64_t, std::shared_ptr<api::IReadIterator<int>>, std::shared_ptr<api::IReadIterator<int>>> {

    std::shared_ptr<api::IReadIterator<int>>
    call(int64_t &t1, std::shared_ptr<api::IReadIterator<int>> &t2, ignis::executor::api::IContext &context) override {
        if (t1 == 0 && t2->hasNext()) {
            t2->next();
        }
        return t2;
    }

};

ignis_register_class(mapPartitionFunctionWithIndex, mapPartitionFunctionWithIndex);

class mapFunctionWithArgs : public function::IFunction<int, std::string> {
public:

    std::string call(int &t, ignis::executor::api::IContext &context) override {
        return std::to_string(t) + *context.getVariable<std::string>("test_var");
    }

};

ignis_register_class(mapFunctionWithArgs, mapFunctionWithArgs);