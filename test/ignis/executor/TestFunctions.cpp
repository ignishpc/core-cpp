
#include <string>
#include <vector>
#include "ignis/executor/api/IReadIterator.h"
#include "ignis/executor/api/function/IFunction0.h"
#include "ignis/executor/api/function/IFunction.h"
#include "ignis/executor/api/function/IFunction2.h"
#include "ignis/executor/api/function/IVoidFunction.h"
#include "ignis/executor/api/function/IVoidFunction2.h"

using namespace ignis::executor::api;

class MapInt : public function::IFunction<int, std::string> {
public:
    std::string call(int &v, IContext &context) override {
        return std::to_string(v);
    }
};

ignis_export(MapInt, MapInt)


class FilterInt : public function::IFunction<int, bool> {
public:
    bool call(int &v, IContext &context) override {
        return v % 2 == 0;
    }
};

ignis_export(FilterInt, FilterInt)

class FlatmapInt : public function::IFunction<int, std::vector<int>> {
public:
    std::vector<int> call(int &v, IContext &context) override {
        return std::vector<int>(2, v);
    }
};

ignis_export(FlatmapInt, FlatmapInt)

class MapPartitionsInt : public function::IFunction<IReadIterator<int>, std::vector<std::string>> {
public:
    std::vector<std::string> call(IReadIterator<int> &it, IContext &context) override {
        std::vector<std::string> v;
        while (it.hasNext()) {
            v.push_back(std::to_string(it.next()));
        }
        return v;
    }
};

ignis_export(MapPartitionsInt, MapPartitionsInt)

class MapPartitionWithIndexInt : public function::IFunction2<int64_t, IReadIterator<int>, std::vector<std::string>> {
public:
    std::vector<std::string> call(int64_t &p, IReadIterator<int> &it, IContext &context) override {
        std::vector<std::string> v;
        while (it.hasNext()) {
            v.push_back(std::to_string(it.next()));
        }
        return v;
    }
};

ignis_export(MapPartitionWithIndexInt, MapPartitionWithIndexInt)

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

class SortInt : public function::IFunction2<int, int, bool> {
public:
    bool call(int &v1, int &v2, IContext &context) override {
        return v1 < v2;
    }
};

ignis_export(SortInt, SortInt)


class SortString : public function::IFunction2<std::string, std::string, bool> {
public:
    bool call(std::string &v1, std::string &v2, IContext &context) override {
        return v1 < v2;
    }
};

ignis_export(SortString, SortString)


class ZeroInt : public function::IFunction0<int> {
public:
    int call(IContext &context) override {
        return 0;
    }

};

ignis_export(ZeroInt, ZeroInt)

class ReduceIntToString : public function::IFunction2<std::string, int, std::string> {
public:
    std::string call(std::string &v1, int &v2, IContext &context) override {
        return v1 + std::to_string(v2);
    }
};

ignis_export(ReduceIntToString, ReduceIntToString)

class ZeroString : public function::IFunction0<std::string> {
public:
    std::string call(IContext &context) override {
        return "";
    }

};

ignis_export(ZeroString, ZeroString)

class ForeachInt : public function::IVoidFunction<int> {
public:
    void call(int &elem, IContext &context) override {
    }

};

ignis_export(ForeachInt, ForeachInt)

class ForeachPartitionString : public function::IVoidFunction<IReadIterator<std::string>> {
public:
    void call(IReadIterator<std::string> &it, IContext &context) override {
        while(it.hasNext()){
            it.next();
        }
    }

};

ignis_export(ForeachPartitionString, ForeachPartitionString)