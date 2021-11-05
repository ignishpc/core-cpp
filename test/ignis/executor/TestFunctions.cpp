
#include "ignis/executor/api/IReadIterator.h"
#include "ignis/executor/api/function/IFunction.h"
#include "ignis/executor/api/function/IFunction0.h"
#include "ignis/executor/api/function/IFunction2.h"
#include "ignis/executor/api/function/IVoidFunction.h"
#include "ignis/executor/api/function/IVoidFunction0.h"
#include "ignis/executor/api/function/IVoidFunction2.h"
#include "ignis/executor/api/function/IBeforeFunction.h"
#include <string>
#include <vector>

using namespace ignis::executor::api;

class IntSequence : public function::IFunction0<IVector<IVector<int>>> {
public:
    IVector<IVector<int>> call(IContext &context) override {
        IVector<int> result;
        for (int i = 0; i < 100; i++) { result.push_back(i); }
        return {result};
    }
};

ignis_export(IntSequence, IntSequence)

class NoneFunction : public function::IVoidFunction0 {
public:
    void call(IContext &context) override { context.var<bool>("NoneFunction") = true; }
};

ignis_export(NoneFunction, NoneFunction)

class MapInt : public function::IFunction<int, std::string> {
public:
    std::string call(int &v, IContext &context) override { return std::to_string(v); }
};

ignis_export(MapInt, MapInt)


class FilterInt : public function::IFunction<int, bool> {
public:
    bool call(int &v, IContext &context) override { return v % 2 == 0; }
};

ignis_export(FilterInt, FilterInt)

class FlatmapString : public function::IFunction<std::string, std::vector<std::string>> {
public:
    std::vector<std::string> call(std::string &v, IContext &context) override { return std::vector<std::string>(2, v); }
};

ignis_export(FlatmapString, FlatmapString)

class KeyByString : public function::IFunction<std::string, int> {
public:
    int call(std::string &v, IContext &context) override { return (int) v.length(); }
};

ignis_export(KeyByString, KeyByString)

class MapPartitionsInt : public function::IFunction<IReadIterator<int>, std::vector<std::string>> {
public:
    std::vector<std::string> call(IReadIterator<int> &it, IContext &context) override {
        std::vector<std::string> v;
        while (it.hasNext()) { v.push_back(std::to_string(it.next())); }
        return v;
    }
};

ignis_export(MapPartitionsInt, MapPartitionsInt)

class MapPartitionWithIndexInt : public function::IFunction2<int64_t, IReadIterator<int>, std::vector<std::string>> {
public:
    std::vector<std::string> call(int64_t &p, IReadIterator<int> &it, IContext &context) override {
        std::vector<std::string> v;
        while (it.hasNext()) { v.push_back(std::to_string(it.next())); }
        return v;
    }
};

ignis_export(MapPartitionWithIndexInt, MapPartitionWithIndexInt)

class MapExecutorInt : public function::IVoidFunction<IVector<IVector<int> *>> {
public:
    void call(IVector<IVector<int> *> &parts, IContext &context) override {
        for (auto &part : parts) {
            for (auto &elem : *part) { elem++; }
        }
    }
};

ignis_export(MapExecutorInt, MapExecutorInt)

class MapExecutorToString : public function::IFunction<IVector<IVector<int> *>, IVector<IVector<std::string>>> {
public:
    IVector<IVector<std::string>> call(IVector<IVector<int> *> &parts, IContext &context) override {
        IVector<IVector<std::string>> v;
        for (auto part : parts) {
            v.emplace_back();
            for (auto elem : *part) { v.back().push_back(std::to_string(elem)); }
        }
        return std::move(v);
    }
};

ignis_export(MapExecutorToString, MapExecutorToString)

class GroupByIntString : public function::IFunction<std::string, int> {
public:
    int call(std::string &elem, IContext &context) override { return (int) elem.length(); }
};

ignis_export(GroupByIntString, GroupByIntString)

class ReduceInt : public function::IFunction2<int, int, int> {
public:
    int call(int &v1, int &v2, IContext &context) override { return v1 + v2; }
};

ignis_export_with_key(ReduceInt, ReduceInt, int)


class ReduceString : public function::IFunction2<std::string, std::string, std::string> {
public:
    std::string call(std::string &v1, std::string &v2, IContext &context) override { return v1 + v2; }
};

ignis_export_with_key(ReduceString, ReduceString, int)

class SortInt : public function::IFunction2<int, int, bool> {
public:
    bool call(int &v1, int &v2, IContext &context) override { return v1 < v2; }
};

ignis_export(SortInt, SortInt)


class SortString : public function::IFunction2<std::string, std::string, bool> {
public:
    bool call(std::string &v1, std::string &v2, IContext &context) override { return v1 < v2; }
};

ignis_export(SortString, SortString)


class MapValuesInt : public function::IFunction<int, std::string> {
public:
    std::string call(int &v, IContext &context) override { return std::to_string(v); }
};

ignis_export_with_key(MapValuesInt, MapValuesInt, int)

class FlatMapValuesInt : public function::IFunction<int, std::vector<std::string>> {
public:
    std::vector<std::string> call(int &v, IContext &context) override {
        return std::vector<std::string>(1, std::to_string(v));
    }
};

ignis_export_with_key(FlatMapValuesInt, FlatMapValuesInt, int)

class ZeroInt : public function::IFunction0<int> {
public:
    int call(IContext &context) override { return 0; }
};

ignis_export(ZeroInt, ZeroInt)

class ReduceIntToString : public function::IFunction2<std::string, int, std::string> {
public:
    std::string call(std::string &v1, int &v2, IContext &context) override { return v1 + std::to_string(v2); }
};

ignis_export_with_key(ReduceIntToString, ReduceIntToString, int)

class ZeroString : public function::IFunction0<std::string> {
public:
    std::string call(IContext &context) override { return ""; }
};

ignis_export(ZeroString, ZeroString)

class ForeachInt : public function::IVoidFunction<int> {
public:
    void call(int &elem, IContext &context) override { context.var<bool>("ForeachInt") = true; }
};

ignis_export(ForeachInt, ForeachInt)

class ForeachPartitionString : public function::IVoidFunction<IReadIterator<std::string>> {
public:
    void call(IReadIterator<std::string> &it, IContext &context) override {
        while (it.hasNext()) { it.next(); }
        context.var<bool>("ForeachPartitionString") = true;
    }
};

ignis_export(ForeachPartitionString, ForeachPartitionString)

class ForeachExecutorString : public function::IVoidFunction<IVector<IVector<std::string> *>> {
public:
    void call(IVector<IVector<std::string> *> &parts, IContext &context) override {
        context.var<bool>("ForeachExecutorString") = true;
    }
};

ignis_export(ForeachExecutorString, ForeachExecutorString);

class TypeInt : public function::IBeforeFunction<int> {
};

ignis_export(TypeInt, TypeInt)

class TypeString : public function::IBeforeFunction<std::string> {
};

ignis_export(TypeString, TypeString)

class PartitionByStr : public function::IFunction<std::string, int64_t> {
public:
    int64_t call(std::string &v, IContext &context) override { return hash(v); }
    std::hash<std::string> hash;
};

ignis_export(PartitionByStr, PartitionByStr);