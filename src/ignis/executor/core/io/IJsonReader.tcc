
#include "IJsonReader.h"

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
struct ignis::executor::core::io::IJsonReaderType<bool> {
    inline void operator()(JsonNode &in, bool &obj) {
        obj = (*this)(in);
    }

    inline bool operator()(JsonNode &in) {
        checkJsonTypeAux<bool>(in.IsBool());
        return in.GetBool();
    }
};

template<>
struct ignis::executor::core::io::IJsonReaderType<int8_t> {
    inline void operator()(JsonNode &in, int8_t &obj) {
        obj = (*this)(in);
    }

    inline int8_t operator()(JsonNode &in) {
        checkJsonTypeAux<int8_t>(in.IsInt());
        return in.GetInt();
    }
};

template<>
struct ignis::executor::core::io::IJsonReaderType<uint8_t> {
    inline void operator()(JsonNode &in, uint8_t &obj) {
        obj = (*this)(in);
    }

    inline uint8_t operator()(JsonNode &in) {
        checkJsonTypeAux<uint8_t>(in.IsUint());
        return in.GetUint();
    }
};

template<>
struct ignis::executor::core::io::IJsonReaderType<int16_t> {
    inline void operator()(JsonNode &in, int16_t &obj) {
        obj = (*this)(in);
    }

    inline int16_t operator()(JsonNode &in) {
        checkJsonTypeAux<int16_t>(in.IsInt());
        return in.GetInt();
    }
};

template<>
struct ignis::executor::core::io::IJsonReaderType<uint16_t> {
    inline void operator()(JsonNode &in, uint16_t &obj) {
        obj = (*this)(in);
    }

    inline uint16_t operator()(JsonNode &in) {
        checkJsonTypeAux<uint16_t>(in.IsUint());
        return in.GetUint();
    }
};

template<>
struct ignis::executor::core::io::IJsonReaderType<int32_t> {
    inline void operator()(JsonNode &in, int32_t &obj) {
        obj = (*this)(in);
    }

    inline int32_t operator()(JsonNode &in) {
        checkJsonTypeAux<int32_t>(in.IsInt());
        return in.GetInt();
    }
};

template<>
struct ignis::executor::core::io::IJsonReaderType<uint32_t> {
    inline void operator()(JsonNode &in, uint32_t &obj) {
        obj = (*this)(in);
    }

    inline uint32_t operator()(JsonNode &in) {
        checkJsonTypeAux<uint32_t>(in.IsUint());
        return in.GetUint();
    }
};

template<>
struct ignis::executor::core::io::IJsonReaderType<int64_t> {
    inline void operator()(JsonNode &in, int64_t &obj) {
        obj = (*this)(in);
    }

    inline int64_t operator()(JsonNode &in) {
        checkJsonTypeAux<uint32_t>(in.IsInt64());
        return in.GetInt64();
    }
};

template<>
struct ignis::executor::core::io::IJsonReaderType<uint64_t> {
    inline void operator()(JsonNode &in, uint64_t &obj) {
        obj = (*this)(in);
    }

    inline uint64_t operator()(JsonNode &in) {
        checkJsonTypeAux<uint32_t>(in.IsUint64());
        return in.GetUint();
    }
};

template<>
struct ignis::executor::core::io::IJsonReaderType<float> {
    inline void operator()(JsonNode &in, float &obj) {
        obj = (*this)(in);
    }

    inline float operator()(JsonNode &in) {
        checkJsonTypeAux<float>(in.IsFloat());
        return in.GetFloat();
    }
};

template<>
struct ignis::executor::core::io::IJsonReaderType<double> {
    inline void operator()(JsonNode &in, double &obj) {
        obj = (*this)(in);
    }

    inline double operator()(JsonNode &in) {
        checkJsonTypeAux<double>(in.IsDouble());
        return in.GetDouble();
    }
};

template<>
struct ignis::executor::core::io::IJsonReaderType<std::string> {
    inline void operator()(JsonNode &in, std::string &obj) {
        obj = (*this)(in);
    }

    inline std::string operator()(JsonNode &in) {
        checkJsonTypeAux<std::string>(in.IsString());
        return in.GetString();
    }
};

template<typename _Tp, typename _Alloc>
struct ignis::executor::core::io::IJsonReaderType<std::vector<_Tp, _Alloc>> {
    inline void operator()(JsonNode &in, std::vector<_Tp, _Alloc> &obj) {
        checkJsonTypeAux<std::vector<_Tp, _Alloc>>(in.IsArray());
        IJsonReaderType<_Tp> reader;
        auto array = in.GetArray();
        obj.reserve(array.Size());
        for (auto &value: array) {
            obj.push_back(reader(value));
        }
    }

    inline std::vector<_Tp, _Alloc> operator()(JsonNode &in) {
        std::vector<_Tp, _Alloc> obj;
        (*this)(in, obj);
        return std::move(obj);
    }
};

template<typename _Tp, typename _Alloc>
struct ignis::executor::core::io::IJsonReaderType<std::list<_Tp, _Alloc>> {
    inline void operator()(JsonNode &in, std::list<_Tp, _Alloc> &obj) {
        checkJsonTypeAux<std::list<_Tp, _Alloc>>(in.IsArray());
        IJsonReaderType<_Tp> reader;
        auto array = in.GetArray();
        for (auto &value: array) {
            obj.push_back(reader(value));
        }
    }

    inline std::list<_Tp, _Alloc> operator()(JsonNode &in) {
        std::list<_Tp, _Alloc> obj;
        (*this)(in, obj);
        return std::move(obj);
    }
};

template<typename _Tp, typename _Alloc>
struct ignis::executor::core::io::IJsonReaderType<std::forward_list<_Tp, _Alloc>> {
    inline void operator()(JsonNode &in, std::forward_list<_Tp, _Alloc> &obj) {
        checkJsonTypeAux<std::forward_list<_Tp, _Alloc>>(in.IsArray());
        IJsonReaderType<_Tp> reader;
        auto array = in.GetArray();
        auto it = obj.before_begin();
        for (auto &value: array) {
            it = obj.insert_after(it, reader(value));
        }
    }

    inline std::forward_list<_Tp, _Alloc> operator()(JsonNode &in) {
        std::forward_list<_Tp, _Alloc> obj;
        (*this)(in, obj);
        return std::move(obj);
    }
};

template<typename _Key, typename _Compare, typename _Alloc>
struct ignis::executor::core::io::IJsonReaderType<std::set<_Key, _Compare, _Alloc>> {
    inline void operator()(JsonNode &in, std::set<_Key, _Compare, _Alloc> &obj) {
        checkJsonTypeAux<std::set<_Key, _Compare, _Alloc>>(in.IsArray());
        IJsonReaderType<_Key> reader;
        auto array = in.GetArray();
        for (auto &value: array) {
            obj.insert(reader(value));
        }
    }

    inline std::set<_Key, _Compare, _Alloc> operator()(JsonNode &in) {
        std::set<_Key, _Compare, _Alloc> obj;
        (*this)(in, obj);
        return std::move(obj);
    }
};

template<typename _Value, typename _Hash, typename _Pred, typename _Alloc>
struct ignis::executor::core::io::IJsonReaderType<std::unordered_set<_Value, _Hash, _Pred, _Alloc>> {
    inline void operator()(JsonNode &in, std::unordered_set<_Value, _Hash, _Pred, _Alloc> &obj) {
        checkJsonTypeAux<std::unordered_set<_Value, _Hash, _Pred, _Alloc>>(in.IsArray());
        IJsonReaderType<_Value> reader;
        auto array = in.GetArray();
        obj.reserve(array.Size());
        for (auto &value: array) {
            obj.insert(reader(value));
        }
    }

    inline std::unordered_set<_Value, _Hash, _Pred, _Alloc> operator()(JsonNode &in) {
        std::unordered_set<_Value, _Hash, _Pred, _Alloc> obj;
        (*this)(in, obj);
        return std::move(obj);
    }
};

template<typename _Key, typename _Tp, typename _Compare, typename _Alloc>
struct ignis::executor::core::io::IJsonReaderType<std::map<_Key, _Tp, _Compare, _Alloc>> {
    inline void operator()(JsonNode &in, std::map<_Key, _Tp, _Compare, _Alloc> &obj) {
        IJsonReaderType<_Key> key_reader;
        IJsonReaderType<_Tp> value_reader;
        if (std::is_convertible<_Key, std::string>()) {
            checkJsonTypeAux<std::map<_Key, _Tp, _Compare, _Alloc>>(in.IsObject());
            auto map = in.GetObject();
            for (auto &entry:map) {
                obj[key_reader(entry.name)] = value_reader(entry.value);
            }
        } else {
            checkJsonTypeAux<std::map<_Key, _Tp, _Compare, _Alloc>>(in.IsArray());
            auto array = in.GetArray();
            for (auto &elem:array) {
                checkJsonTypeAux<std::map<_Key, _Tp, _Compare, _Alloc>>(elem.IsObject() &&
                                                                        elem.MemberCount() == 2 &&
                                                                        elem.HasMember("key") &&
                                                                        elem.HasMember("value"));
                auto& key = elem["key"];
                auto& value = elem["value"];
                obj[key_reader(key)] = value_reader(value);
            }
        }
    }

    inline std::map<_Key, _Tp, _Compare, _Alloc> operator()(JsonNode &in) {
        std::map<_Key, _Tp, _Compare, _Alloc> obj;
        (*this)(in, obj);
        return std::move(obj);
    }
};

template<typename _Key, typename _Tp, typename _Hash, typename _Pred, typename _Alloc>
struct ignis::executor::core::io::IJsonReaderType<std::unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc>> {
    inline void operator()(JsonNode &in, std::unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc> &obj) {
        IJsonReaderType<_Key> key_reader;
        IJsonReaderType<_Tp> value_reader;
        if (std::is_convertible<_Key, std::string>()) {
            checkJsonTypeAux<std::unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc>>(in.IsObject());
            auto map = in.GetObject();
            for (auto &entry:map) {
                obj[key_reader(entry.name)] = value_reader(entry.value);
            }
        } else {
            checkJsonTypeAux<std::unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc>>(in.IsArray());
            auto array = in.GetArray();
            for (auto &elem:array) {
                checkJsonTypeAux<std::unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc>>(elem.IsObject() &&
                                                                                      elem.MemberCount() == 2 &&
                                                                                      elem.HasMember("key") &&
                                                                                      elem.HasMember("value"));
                auto& key = elem["key"];
                auto& value = elem["value"];
                obj[key_reader(key)] = value_reader(value);
            }
        }
    }

    inline std::unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc> operator()(JsonNode &in) {
        std::unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc> obj;
        (*this)(in, obj);
        return std::move(obj);
    }
};

template<typename _T1, typename _T2>
struct ignis::executor::core::io::IJsonReaderType<std::pair<_T1, _T2>> {
    inline void operator()(JsonNode &in, std::pair<_T1, _T2> &obj) {
        checkJsonTypeAux<std::pair<_T1, _T2>>(in.IsObject());
        auto json_object = in.GetObject();
        if (std::is_convertible<_T1, std::string>()) {
            checkJsonTypeAux<std::pair<_T1, _T2>>(json_object.MemberCount() == 1);
            key_reader(json_object.begin()->name, obj.first);
            value_reader(json_object.begin()->value, obj.second);
        } else {
            checkJsonTypeAux<std::pair<_T1, _T2>>(json_object.MemberCount() == 2 &&
                                                  json_object.HasMember("first") &&
                                                  json_object.HasMember("second"));
            key_reader(json_object["first"], obj.first);
            value_reader(json_object["second"], obj.second);
        }
    }

    inline std::pair<_T1, _T2> operator()(JsonNode &in) {
        std::pair<_T1, _T2> obj;
        (*this)(in, obj);
        return std::move(obj);
    }

private:
    IJsonReaderType <_T1> key_reader;
    IJsonReaderType <_T2> value_reader;
};

template<typename _Tp>
struct ignis::executor::core::io::IJsonReaderType<_Tp *> {
    inline void operator()(JsonNode &in, _Tp *&obj) {
        reader(in, *obj);
    }

    inline _Tp *operator()(JsonNode &in) {
        _Tp *obj;
        (*this)(in, obj);
        return std::move(obj);
    }

private:
    IJsonReaderType <_Tp> reader;
};

template<typename _Tp>
struct ignis::executor::core::io::IJsonReaderType<std::shared_ptr<_Tp>> {
    inline void operator()(JsonNode &in, std::shared_ptr<_Tp> &obj) {
        reader(in, *obj);
    }

    inline std::shared_ptr<_Tp> operator()(JsonNode &in) {
        std::shared_ptr<_Tp> obj;
        (*this)(in, obj);
        return std::move(obj);
    }

private:
    IJsonReaderType <_Tp> reader;
};
