
#ifndef IGNIS_IPRINTER_H
#define IGNIS_IPRINTER_H

#include <string>
#include <utility>
#include <vector>
#include <list>
#include <forward_list>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <memory>
#include <ostream>
#include <jsoncpp/json/json.h>
#include "../../exception/ILogicError.h"
#include "../RTTInfo.h"

namespace ignis {
    namespace data {
        namespace serialization {

            std::string inline tab(size_t n) { return std::string(n, '\t'); }

            template<typename T>
            struct IPrinter {
                void operator()(const T &b, std::ostream &out, size_t level = 0) {
                    throw exception::ILogicError(
                            "IPrinter not implemented for " + RTTInfo(typeid(T)).getStandardName());
                }

                void printJson(const T &b, Json::Value &json) {
                    throw exception::ILogicError(
                            "IPrinter json not implemented for " + RTTInfo(typeid(T)).getStandardName());
                }
            };

            template<>
            struct IPrinter<bool> {
                inline void operator()(const bool &b, std::ostream &out, size_t level = 0) {
                    out << b;
                }

                inline void printJson(const bool &b, Json::Value &json) {
                    json = Json::Value(b);
                }
            };

            template<>
            struct IPrinter<int8_t> {
                inline void operator()(const int8_t &i, std::ostream &out, size_t level = 0) {
                    out << i;
                }

                inline void printJson(const int8_t &i, Json::Value &json) {
                    json = Json::Value(i);
                }
            };

            template<>
            struct IPrinter<uint8_t> {
                inline void operator()(const uint8_t &i, std::ostream &out, size_t level = 0) {
                    out << i;
                }

                inline void printJson(const uint8_t &i, Json::Value &json) {
                    json = Json::Value(i);
                }
            };

            template<>
            struct IPrinter<int16_t> {
                inline void operator()(const int16_t &i, std::ostream &out, size_t level = 0) {
                    out << i;
                }

                inline void printJson(const int16_t &i, Json::Value &json) {
                    json = Json::Value(i);
                }
            };

            template<>
            struct IPrinter<uint16_t> {
                inline void operator()(const uint16_t &i, std::ostream &out, size_t level = 0) {
                    out << i;
                }

                inline void printJson(const uint16_t &i, Json::Value &json) {
                    json = Json::Value(i);
                }
            };

            template<>
            struct IPrinter<int32_t> {
                inline void operator()(const int32_t &i, std::ostream &out, size_t level = 0) {
                    out << i;
                }

                inline void printJson(const int32_t &i, Json::Value &json) {
                    json = Json::Value(i);
                }
            };

            template<>
            struct IPrinter<uint32_t> {
                inline void operator()(const uint32_t &i, std::ostream &out, size_t level = 0) {
                    out << i;
                }

                inline void printJson(const uint32_t &i, Json::Value &json) {
                    json = Json::Value(i);
                }
            };

            template<>
            struct IPrinter<int64_t> {
                inline void operator()(const int64_t &i, std::ostream &out, size_t level = 0) {
                    out << i;
                }

                inline void printJson(const int64_t &i, Json::Value &json) {
                    json = Json::Value((Json::Int64) i);
                }
            };

            template<>
            struct IPrinter<uint64_t> {
                inline void operator()(const uint64_t &i, std::ostream &out, size_t level = 0) {
                    out << i;
                }

                inline void printJson(const uint64_t &i, Json::Value &json) {
                    json = Json::Value((Json::Int64) i);
                }
            };

            template<>
            struct IPrinter<float> {
                inline void operator()(const float &d, std::ostream &out, size_t level = 0) {
                    out << d;
                }

                inline void printJson(const float &d, Json::Value &json) {
                    json = Json::Value(d);
                }
            };

            template<>
            struct IPrinter<double> {
                inline void operator()(const double &d, std::ostream &out, size_t level = 0) {
                    out << d;
                }

                inline void printJson(const double &d, Json::Value &json) {
                    json = Json::Value(d);
                }
            };

            template<>
            struct IPrinter<long double> {
                inline void operator()(const long double &d, std::ostream &out, size_t level = 0) {
                    out << d;
                }

                inline void printJson(const long double &d, Json::Value &json) {
                    json = Json::Value(std::to_string(d));
                }
            };

            template<>
            struct IPrinter<std::string> {
                inline void operator()(const std::string &s, std::ostream &out, size_t level = 0) {
                    out << s;
                }

                inline void printJson(const std::string &s, Json::Value &json) {
                    json = Json::Value(s);
                }
            };

            template<typename T>
            struct IPrinter<std::vector<T>> {
                inline void operator()(const std::vector<T> &v, std::ostream &out, size_t level = 0) {
                    for (const auto &elem:v) {
                        out << tab(level);
                        IPrinter<T>()(elem, out, level + 1);
                        out << std::endl;
                    }
                }

                inline void printJson(const std::vector<T> &v, Json::Value &json) {
                    json = Json::Value(Json::arrayValue);
                    json.resize(v.size());
                    Json::ArrayIndex i = 0;
                    for (const auto &elem:v) {
                        Json::Value child;
                        IPrinter<T>().printJson(elem, child);
                        json[i++] = child;
                    }

                }
            };

            template<typename T>
            struct IPrinter<std::list<T>> {
                inline void operator()(const std::list<T> &l, std::ostream &out, size_t level = 0) {
                    for (const auto &elem:l) {
                        out << tab(level);
                        IPrinter<T>()(elem, out, level + 1);
                        out << std::endl;
                    }
                }

                inline void printJson(const std::list<T> &l, Json::Value &json) {
                    json = Json::Value(Json::arrayValue);
                    json.resize(l.size());
                    Json::ArrayIndex i = 0;
                    for (const auto &elem:l) {
                        Json::Value child;
                        IPrinter<T>().printJson(elem, child);
                        json[i++] = child;
                    }

                }
            };

            template<typename T>
            struct IPrinter<std::forward_list<T>> {
                inline void operator()(const std::forward_list<T> &fl, std::ostream &out, size_t level = 0) {
                    for (const auto &elem:fl) {
                        out << tab(level);
                        IPrinter<T>()(elem, out, level + 1);
                        out << std::endl;
                    }
                }

                inline void printJson(const std::forward_list<T> &fl, Json::Value &json) {
                    json = Json::Value(Json::arrayValue);
                    for (const auto &elem:fl) {
                        Json::Value child;
                        IPrinter<T>().printJson(elem, child);
                        json.append(child);
                    }

                }
            };

            template<typename T>
            struct IPrinter<std::set<T>> {
                inline void operator()(const std::set<T> &s, std::ostream &out, size_t level = 0) {
                    for (const auto &elem:s) {
                        out << tab(level);
                        IPrinter<T>()(elem, out, level + 1);
                        out << std::endl;
                    }
                }

                inline void printJson(const std::set<T> &s, Json::Value &json) {
                    json = Json::Value(Json::arrayValue);
                    json.resize(s.size());
                    Json::ArrayIndex i = 0;
                    for (const auto &elem:s) {
                        Json::Value child;
                        IPrinter<T>().printJson(elem, child);
                        json[i++] = child;
                    }

                }
            };

            template<typename T>
            struct IPrinter<std::unordered_set<T>> {
                inline void operator()(const std::set<T> &us, std::ostream &out, size_t level = 0) {
                    for (const auto &elem:us) {
                        out << tab(level);
                        IPrinter<T>()(elem, out, level + 1);
                        out << std::endl;
                    }
                }

                inline void printJson(const std::unordered_set<T> &us, Json::Value &json) {
                    json = Json::Value(Json::arrayValue);
                    json.resize(us.size());
                    Json::ArrayIndex i = 0;
                    for (const auto &elem:us) {
                        Json::Value child;
                        IPrinter<T>().printJson(elem, child);
                        json[i++] = child;
                    }

                }
            };

            template<typename K, typename V>
            struct IPrinter<std::map<K, V>> {
                inline void operator()(const std::map<K, V> &m, std::ostream &out, size_t level = 0) {
                    for (const auto &elem:m) {
                        out << tab(level) << "(";
                        IPrinter<K>()(elem.first, out, level + 1);
                        out << ", ";
                        IPrinter<K>()(elem.second, out, level + 1);
                        out << ")" << std::endl;
                    }
                }

                inline void printJson(const std::map<K, V> &m, Json::Value &json) {
                    json = Json::Value(Json::objectValue);
                    if (std::is_convertible<Json::UInt64, K>() ||
                        std::is_convertible<double, K>() ||
                        std::is_convertible<std::string, K>()) {
                        for (const auto &elem:m) {
                            Json::Value child;
                            IPrinter<V>().printJson(elem.second, child);
                            json[elem.first] = child;
                        }
                    } else {
                        for (const auto &elem:m) {
                            Json::Value key;
                            Json::Value value;
                            IPrinter<K>().printJson(elem.first, key);
                            IPrinter<V>().printJson(elem.second, value);
                            json["key"] = key;
                            json["value"] = value;
                        }
                    }
                }
            };

            template<typename K, typename V>
            struct IPrinter<std::unordered_map<K, V>> {
                inline void operator()(const std::unordered_map<K, V> &m, std::ostream &out, size_t level = 0) {
                    for (const auto &elem:m) {
                        out << tab(level) << "(";
                        IPrinter<K>()(elem.first, out, level + 1);
                        out << ", ";
                        IPrinter<K>()(elem.second, out, level + 1);
                        out << ")" << std::endl;
                    }
                }

                inline void printJson(const std::unordered_map<K, V> &m, Json::Value &json) {
                    json = Json::Value(Json::objectValue);
                    if (std::is_convertible<Json::UInt64, K>() ||
                        std::is_convertible<double, K>() ||
                        std::is_convertible<std::string, K>()) {
                        for (const auto &elem:m) {
                            Json::Value child;
                            IPrinter<V>().printJson(elem.second, child);
                            json[elem.first] = child;
                        }
                    } else {
                        for (const auto &elem:m) {
                            Json::Value key;
                            Json::Value value;
                            IPrinter<K>().printJson(elem.first, key);
                            IPrinter<V>().printJson(elem.second, value);
                            json["key"] = key;
                            json["value"] = value;
                        }
                    }
                }
            };

            template<typename T1, typename T2>
            struct IPrinter<std::pair<T1, T2>> {
                inline void operator()(const std::pair<T1, T2> &p, std::ostream &out, size_t level = 0) {
                    out << tab(level) << "(";
                    IPrinter<T1>()(p.first, out, level + 1);
                    out << ", ";
                    IPrinter<T2>()(p.second, out, level + 1);
                    out << ")" << std::endl;
                }

                inline void printJson(std::pair<T1, T2> &p, Json::Value &json) {
                    json = Json::Value(Json::objectValue);
                    if (std::is_convertible<Json::UInt64, T1>() ||
                        std::is_convertible<double, T1>() ||
                        std::is_convertible<std::string, T1>()) {
                        Json::Value child;
                        IPrinter<T2>().printJson(p.second, child);
                        json[p.first] = child;
                    } else {
                        Json::Value first;
                        Json::Value second;
                        IPrinter<T1>().printJson(p.first, first);
                        IPrinter<T2>().printJson(p.second, second);
                        json["first"] = first;
                        json["second"] = second;
                    }
                }
            };

            template<typename T>
            struct IPrinter<T*> {
                inline void operator()(const T* &v, std::ostream &out, size_t level = 0) {
                   IPrinter<T*>()(*v,out);
                }

                inline void printJson(const T* &v, Json::Value &json) {
                    IPrinter<T*>().printJson(*v,json);
                }
            };

            template<typename T>
            struct IPrinter<std::shared_ptr<T>> {
                inline void operator()(const std::shared_ptr<T> &v, std::ostream &out, size_t level = 0) {
                    IPrinter<T*>()(*v,out);
                }

                inline void printJson(const std::shared_ptr<T> &v, Json::Value &json) {
                    IPrinter<T*>().printJson(*v,json);
                }
            };
        }
    }
}

#endif
