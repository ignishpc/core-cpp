
#include "IJsonWriter.h"

#include "ignis/executor/api/IReadIterator.h"
#include <forward_list>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

template<>
struct ignis::executor::core::io::IJsonWriterType<bool> {
    inline void operator()(JsonWriter &out, const bool &b) { out.Bool(b); }
};

template<>
struct ignis::executor::core::io::IJsonWriterType<int8_t> {
    inline void operator()(JsonWriter &out, const int8_t &i) { out.Int(i); }
};

template<>
struct ignis::executor::core::io::IJsonWriterType<uint8_t> {
    inline void operator()(JsonWriter &out, const uint8_t &i) { out.Uint(i); }
};

template<>
struct ignis::executor::core::io::IJsonWriterType<int16_t> {
    inline void operator()(JsonWriter &out, const int16_t &i) { out.Int(i); }
};

template<>
struct ignis::executor::core::io::IJsonWriterType<uint16_t> {
    inline void operator()(JsonWriter &out, const uint16_t &i) { out.Uint(i); }
};

template<>
struct ignis::executor::core::io::IJsonWriterType<int32_t> {
    inline void operator()(JsonWriter &out, const int32_t &i) { out.Int(i); }
};

template<>
struct ignis::executor::core::io::IJsonWriterType<uint32_t> {
    inline void operator()(JsonWriter &out, const uint32_t &i) { out.Uint(i); }
};

template<>
struct ignis::executor::core::io::IJsonWriterType<int64_t> {
    inline void operator()(JsonWriter &out, const int64_t &i) { out.Int64(i); }
};

template<>
struct ignis::executor::core::io::IJsonWriterType<uint64_t> {
    inline void operator()(JsonWriter &out, const uint64_t &i) { out.Uint64(i); }
};

template<>
struct ignis::executor::core::io::IJsonWriterType<float> {
    inline void operator()(JsonWriter &out, const float &d) { out.Double(d); }
};

template<>
struct ignis::executor::core::io::IJsonWriterType<double> {
    inline void operator()(JsonWriter &out, const double &d) { out.Double(d); }
};

template<>
struct ignis::executor::core::io::IJsonWriterType<std::string> {
    inline void operator()(JsonWriter &out, const std::string &s) { out.String(s.data()); }
};


template<typename _Tp, typename _Alloc>
struct ignis::executor::core::io::IJsonWriterType<std::vector<_Tp, _Alloc>> {
    inline void operator()(JsonWriter &out, const std::vector<_Tp, _Alloc> &v) {
        out.StartArray();
        IJsonWriterType<_Tp> writer;
        for (const auto &elem : v) { writer(out, elem); }
        out.EndArray();
    }
};

template<typename _Tp, typename _Alloc>
struct ignis::executor::core::io::IJsonWriterType<std::list<_Tp, _Alloc>> {
    inline void operator()(JsonWriter &out, const std::list<_Tp, _Alloc> &l) {
        out.StartArray();
        IJsonWriterType<_Tp> writer;
        for (const auto &elem : l) { writer(out, elem); }
        out.EndArray();
    }
};

template<typename _Tp, typename _Alloc>
struct ignis::executor::core::io::IJsonWriterType<std::forward_list<_Tp, _Alloc>> {
    inline void operator()(JsonWriter &out, const std::forward_list<_Tp, _Alloc> &fl) {
        out.StartArray();
        IJsonWriterType<_Tp> writer;
        for (const auto &elem : fl) { writer(out, elem); }
        out.EndArray();
    }
};

template<typename _Key, typename _Compare, typename _Alloc>
struct ignis::executor::core::io::IJsonWriterType<std::set<_Key, _Compare, _Alloc>> {
    inline void operator()(JsonWriter &out, const std::set<_Key, _Compare, _Alloc> &s) {
        out.StartArray();
        IJsonWriterType<_Key> writer;
        for (const auto &elem : s) { writer(out, elem); }
        out.EndArray();
    }
};

template<typename _Value, typename _Hash, typename _Pred, typename _Alloc>
struct ignis::executor::core::io::IJsonWriterType<std::unordered_set<_Value, _Hash, _Pred, _Alloc>> {
    inline void operator()(JsonWriter &out, const std::unordered_set<_Value, _Hash, _Pred, _Alloc> &us) {
        out.StartArray();
        IJsonWriterType<_Value> writer;
        for (const auto &elem : us) { writer(out, elem); }
        out.EndArray();
    }
};

template<typename _Key, typename _Tp, typename _Compare, typename _Alloc>
struct ignis::executor::core::io::IJsonWriterType<std::map<_Key, _Tp, _Compare, _Alloc>> {
    inline void operator()(JsonWriter &out, const std::map<_Key, _Tp, _Compare, _Alloc> &m) {
        if (std::is_convertible<_Key, std::string>()) {
            out.StartObject();
            IJsonWriterType<_Tp> writer;
            for (const auto &elem : m) {
                out.Key(((std::string &) elem.first).data());
                writer(out, elem.second);
            }
            out.EndObject();
        } else {
            out.StartArray();
            IJsonWriterType<_Key> key_writer;
            IJsonWriterType<_Tp> value_writer;
            for (const auto &elem : m) {
                out.StartObject();
                out.Key("key");
                key_writer(out, elem.first);
                out.Key("value");
                value_writer(out, elem.second);
                out.EndObject();
            }
            out.EndArray();
        }
    }
};

template<typename _Key, typename _Tp, typename _Hash, typename _Pred, typename _Alloc>
struct ignis::executor::core::io::IJsonWriterType<std::unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc>> {
    inline void operator()(JsonWriter &out, const std::unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc> &um) {
        if (std::is_convertible<_Key, std::string>()) {
            out.StartObject();
            IJsonWriterType<_Tp> writer;
            for (const auto &elem : um) {
                out.Key(((std::string &) elem.first).data());
                writer(out, elem.second);
            }
            out.EndObject();
        } else {
            out.StartArray();
            IJsonWriterType<_Key> key_writer;
            IJsonWriterType<_Tp> value_writer;
            for (const auto &elem : um) {
                out.StartObject();
                out.Key("key");
                key_writer(out, elem.first);
                out.Key("value");
                value_writer(out, elem.second);
                out.EndObject();
            }
            out.EndArray();
        }
    }
};


template<typename _T1, typename _T2>
struct ignis::executor::core::io::IJsonWriterType<std::pair<_T1, _T2>> {
    inline void operator()(JsonWriter &out, const std::pair<_T1, _T2> &p) {
        out.StartObject();
        if (std::is_convertible<_T1, std::string>()) {
            out.Key(((std::string &) p.first).data());
            IJsonWriterType<_T2>()(out, p.second);

        } else {
            out.Key("first");
            IJsonWriterType<_T1>()(out, p.first);
            out.Key("second");
            IJsonWriterType<_T2>()(out, p.second);
        }
        out.EndObject();
    }
};

template<typename _Tp>
struct ignis::executor::core::io::IJsonWriterType<_Tp *> {
    inline void operator()(JsonWriter &out, const _Tp *v) { IJsonWriterType<_Tp>()(out, *v); }
};

template<typename _Tp>
struct ignis::executor::core::io::IJsonWriterType<std::shared_ptr<_Tp>> {
    inline void operator()(JsonWriter &out, const std::shared_ptr<_Tp> &v) { IJsonWriterType<_Tp>()(out, *v); }
};

template<typename _Tp>
struct ignis::executor::core::io::IJsonWriterType<ignis::executor::api::IReadIterator<_Tp>> {
    inline void operator()(JsonWriter &out, const ignis::executor::api::IReadIterator<_Tp> &it) {
        auto &noconst_it = const_cast<ignis::executor::api::IReadIterator<_Tp> &>(it);
        out.StartArray();
        IJsonWriterType<_Tp> writer;
        while (noconst_it.hasNext()) { writer(out, noconst_it.next()); }
        out.EndArray();
    }
};