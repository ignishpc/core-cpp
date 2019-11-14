
#include "IJsonPrinter.h"

#include <string>
#include <list>
#include <forward_list>
#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <memory>

template<>
struct ignis::executor::core::io::IJsonPrinterType<bool> {
    inline void operator()(const bool &b, JsonStream &out) {
        out.Bool(b);
    }
};

template<>
struct ignis::executor::core::io::IJsonPrinterType<int8_t> {
    inline void operator()(const int8_t &i, JsonStream &out) {
        out.Int(i);
    }
};

template<>
struct ignis::executor::core::io::IJsonPrinterType<uint8_t> {
    inline void operator()(const uint8_t &i, JsonStream &out) {
        out.Uint(i);
    }
};

template<>
struct ignis::executor::core::io::IJsonPrinterType<int16_t> {
    inline void operator()(const int16_t &i, JsonStream &out) {
        out.Int(i);
    }
};

template<>
struct ignis::executor::core::io::IJsonPrinterType<uint16_t> {
    inline void operator()(const uint16_t &i, JsonStream &out) {
        out.Uint(i);
    }
};

template<>
struct ignis::executor::core::io::IJsonPrinterType<int32_t> {
    inline void operator()(const int32_t &i, JsonStream &out) {
        out.Int(i);
    }
};

template<>
struct ignis::executor::core::io::IJsonPrinterType<uint32_t> {
    inline void operator()(const uint32_t &i, JsonStream &out) {
        out.Uint(i);
    }
};

template<>
struct ignis::executor::core::io::IJsonPrinterType<int64_t> {
    inline void operator()(const int64_t &i, JsonStream &out) {
        out.Int64(i);
    }
};

template<>
struct ignis::executor::core::io::IJsonPrinterType<uint64_t> {
    inline void operator()(const uint64_t &i, JsonStream &out) {
        out.Uint64(i);
    }
};

template<>
struct ignis::executor::core::io::IJsonPrinterType<float> {
    inline void operator()(const float &d, JsonStream &out) {
        out.Double(d);
    }
};

template<>
struct ignis::executor::core::io::IJsonPrinterType<double> {
    inline void operator()(const double &d, JsonStream &out) {
        out.Double(d);
    }
};

template<>
struct ignis::executor::core::io::IJsonPrinterType<std::string> {
    inline void operator()(const std::string &s, JsonStream &out) {
        out.String(s.data());
    }
};



template<typename _Tp, typename _Alloc>
struct ignis::executor::core::io::IJsonPrinterType<std::vector<_Tp, _Alloc>> {
inline void operator()(const std::vector<_Tp, _Alloc> &v, JsonStream &out) {
    out.StartArray();
    for (const auto &elem:v) {
        IJsonPrinterType<_Tp>().printJson(elem, out);
    }
    out.EndArray();
}
};

template<typename _Tp, typename _Alloc>
struct ignis::executor::core::io::IJsonPrinterType<std::list<_Tp, _Alloc>> {
inline void operator()(const std::list<_Tp, _Alloc> &l, JsonStream &out) {
    out.StartArray();
    for (const auto &elem:l) {
        IJsonPrinterType<_Tp>().printJson(elem, out);
    }
    out.EndArray();
}
};

template<typename _Tp, typename _Alloc>
struct ignis::executor::core::io::IJsonPrinterType<std::forward_list<_Tp, _Alloc>> {
inline void operator()(const std::forward_list<_Tp, _Alloc> &fl, JsonStream &out) {
    out.StartArray();
    for (const auto &elem:fl) {
        IJsonPrinterType<_Tp>().printJson(elem, out);
    }
    out.EndArray();
}
};

template<typename _Key, typename _Compare>
struct ignis::executor::core::io::IJsonPrinterType<std::set<_Key, _Compare>> {
inline void operator()(const std::set<_Key, _Compare> &s, JsonStream &out) {
    out.StartArray();
    for (const auto &elem:s) {
        IJsonPrinterType<_Key>().printJson(elem, out);
    }
    out.EndArray();
}
};

template<typename _Value, typename _Hash, typename _Pred, typename _Alloc>
struct ignis::executor::core::io::IJsonPrinterType<std::unordered_set<_Value, _Hash, _Pred, _Alloc>> {
inline void operator()(const std::unordered_set<_Value, _Hash, _Pred, _Alloc> &us, JsonStream &out) {
    out.StartArray();
    for (const auto &elem:us) {
        IJsonPrinterType<_Value>().printJson(elem, out);
    }
    out.EndArray();
}
};

template<typename _Key, typename _Tp, typename _Compare, typename _Alloc>
struct ignis::executor::core::io::IJsonPrinterType<std::map<_Key, _Tp, _Compare, _Alloc>> {
inline void operator()(const std::map<_Key, _Tp, _Compare, _Alloc> &m, JsonStream &out) {
    out.StartObject();
    if (std::is_convertible<_Key, std::string>()) {
        for (const auto &elem:m) {
            out.Key(((std::string &) elem.first).data());
            IJsonPrinterType<_Tp>().printJson(elem.second, out);
        }
    } else {
        for (const auto &elem:m) {
            out.Key("key");
            IJsonPrinterType<_Key>().printJson(elem.first, out);
            out.Key("value");
            IJsonPrinterType<_Tp>().printJson(elem.second, out);
        }
    }
    out.EndObject();
}
};

template<typename _Key, typename _Tp, typename _Hash, typename _Pred, typename _Alloc>
struct ignis::executor::core::io::IJsonPrinterType<std::unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc>> {
inline void
operator()(const std::unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc> &um, JsonStream &out) {
    out.StartObject();
    if (std::is_convertible<_Key, std::string>()) {
        for (const auto &elem:um) {
            out.Key(((std::string &) elem.first).data());
            IJsonPrinterType<_Tp>().printJson(elem.second, out);
        }
    } else {
        for (const auto &elem:um) {
            out.Key("key");
            IJsonPrinterType<_Key>().printJson(elem.first, out);
            out.Key("value");
            IJsonPrinterType<_Tp>().printJson(elem.second, out);
        }
    }
    out.EndObject();
}
};


template<typename _T1, typename _T2>
struct ignis::executor::core::io::IJsonPrinterType<std::pair<_T1, _T2>> {
inline void operator()(const std::pair<_T1, _T2> &p, JsonStream &out) {
    out.StartObject();
    if (std::is_convertible<_T1, std::string>()) {
        out.Key(((std::string &) p.first).data());
        IJsonPrinterType<_T2>().printJson(p.second, out);

    } else {
        out.Key("first");
        IJsonPrinterType<_T1>().printJson(p.first, out);
        out.Key("second");
        IJsonPrinterType<_T2>().printJson(p.second, out);
    }
    out.EndObject();
}
};

template<typename _Tp>
struct ignis::executor::core::io::IJsonPrinterType<_Tp *> {
    inline void operator()(const _Tp *&v, JsonStream &out) {
        IJsonPrinterType<_Tp *>().printJson(*v, out);
    }
};

template<typename _Tp>
struct ignis::executor::core::io::IJsonPrinterType<std::shared_ptr<_Tp>> {
inline void operator()(const std::shared_ptr<_Tp> &v, JsonStream &out) {
    IJsonPrinterType<_Tp>().printJson(*v, out);
}
};