
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
#include "../../exceptions/ILogicError.h"
#include "../RTTInfo.h"

namespace ignis {
    namespace data {
        namespace serialization {

            std::string inline tab(size_t n) { return std::string(n, '\t'); }

            template<typename T>
            struct IPrinterType {
                virtual void operator()(const T &b, std::ostream &out, size_t level) {
                    throw exceptions::ILogicError(
                            "IPrinterType not implemented for " + RTTInfo(typeid(T)).getStandardName());
                }

                virtual void printJson(const T &b, Json::Value &json) {
                    throw exceptions::ILogicError(
                            "IPrinterType json not implemented for " + RTTInfo(typeid(T)).getStandardName());
                }
            };

            template<>
            struct IPrinterType<bool> {
                inline void operator()(const bool &b, std::ostream &out, size_t level) {
                    out << b;
                }

                inline void printJson(const bool &b, Json::Value &json) {
                    json = Json::Value(b);
                }
            };

            template<>
            struct IPrinterType<int8_t> {
                inline void operator()(const int8_t &i, std::ostream &out, size_t level) {
                    out << i;
                }

                inline void printJson(const int8_t &i, Json::Value &json) {
                    json = Json::Value(i);
                }
            };

            template<>
            struct IPrinterType<uint8_t> {
                inline void operator()(const uint8_t &i, std::ostream &out, size_t level) {
                    out << i;
                }

                inline void printJson(const uint8_t &i, Json::Value &json) {
                    json = Json::Value(i);
                }
            };

            template<>
            struct IPrinterType<int16_t> {
                inline void operator()(const int16_t &i, std::ostream &out, size_t level) {
                    out << i;
                }

                inline void printJson(const int16_t &i, Json::Value &json) {
                    json = Json::Value(i);
                }
            };

            template<>
            struct IPrinterType<uint16_t> {
                inline void operator()(const uint16_t &i, std::ostream &out, size_t level) {
                    out << i;
                }

                inline void printJson(const uint16_t &i, Json::Value &json) {
                    json = Json::Value(i);
                }
            };

            template<>
            struct IPrinterType<int32_t> {
                inline void operator()(const int32_t &i, std::ostream &out, size_t level) {
                    out << i;
                }

                inline void printJson(const int32_t &i, Json::Value &json) {
                    json = Json::Value(i);
                }
            };

            template<>
            struct IPrinterType<uint32_t> {
                inline void operator()(const uint32_t &i, std::ostream &out, size_t level) {
                    out << i;
                }

                inline void printJson(const uint32_t &i, Json::Value &json) {
                    json = Json::Value(i);
                }
            };

            template<>
            struct IPrinterType<int64_t> {
                inline void operator()(const int64_t &i, std::ostream &out, size_t level) {
                    out << i;
                }

                inline void printJson(const int64_t &i, Json::Value &json) {
                    json = Json::Value((Json::Int64) i);
                }
            };

            template<>
            struct IPrinterType<uint64_t> {
                inline void operator()(const uint64_t &i, std::ostream &out, size_t level) {
                    out << i;
                }

                inline void printJson(const uint64_t &i, Json::Value &json) {
                    json = Json::Value((Json::Int64) i);
                }
            };

            template<>
            struct IPrinterType<float> {
                inline void operator()(const float &d, std::ostream &out, size_t level) {
                    out << d;
                }

                inline void printJson(const float &d, Json::Value &json) {
                    json = Json::Value(d);
                }
            };

            template<>
            struct IPrinterType<double> {
                inline void operator()(const double &d, std::ostream &out, size_t level) {
                    out << d;
                }

                inline void printJson(const double &d, Json::Value &json) {
                    json = Json::Value(d);
                }
            };

            template<>
            struct IPrinterType<long double> {
                inline void operator()(const long double &d, std::ostream &out, size_t level) {
                    out << d;
                }

                inline void printJson(const long double &d, Json::Value &json) {
                    json = Json::Value(std::to_string(d));
                }
            };

            template<>
            struct IPrinterType<std::string> {
                inline void operator()(const std::string &s, std::ostream &out, size_t level) {
                    out << s;
                }

                inline void printJson(const std::string &s, Json::Value &json) {
                    json = Json::Value(s);
                }
            };

            template<typename T>
            struct IPrinterType<std::vector<T>> {
                inline void operator()(const std::vector<T> &v, std::ostream &out, size_t level) {
                    for (const auto &elem:v) {
                        out << tab(level);
                        IPrinterType<T>()(elem, out, level + 1);
                        out << std::endl;
                    }
                }

                inline void printJson(const std::vector<T> &v, Json::Value &json) {
                    json = Json::Value(Json::arrayValue);
                    json.resize(v.size());
                    Json::ArrayIndex i = 0;
                    for (const auto &elem:v) {
                        Json::Value child;
                        IPrinterType<T>().printJson(elem, child);
                        json[i++] = child;
                    }

                }
            };

            template<typename T>
            struct IPrinterType<std::list<T>> {
                inline void operator()(const std::list<T> &l, std::ostream &out, size_t level) {
                    for (const auto &elem:l) {
                        out << tab(level);
                        IPrinterType<T>()(elem, out, level + 1);
                        out << std::endl;
                    }
                }

                inline void printJson(const std::list<T> &l, Json::Value &json) {
                    json = Json::Value(Json::arrayValue);
                    json.resize(l.size());
                    Json::ArrayIndex i = 0;
                    for (const auto &elem:l) {
                        Json::Value child;
                        IPrinterType<T>().printJson(elem, child);
                        json[i++] = child;
                    }

                }
            };

            template<typename T>
            struct IPrinterType<std::forward_list<T>> {
                inline void operator()(const std::forward_list<T> &fl, std::ostream &out, size_t level) {
                    for (const auto &elem:fl) {
                        out << tab(level);
                        IPrinterType<T>()(elem, out, level + 1);
                        out << std::endl;
                    }
                }

                inline void printJson(const std::forward_list<T> &fl, Json::Value &json) {
                    json = Json::Value(Json::arrayValue);
                    for (const auto &elem:fl) {
                        Json::Value child;
                        IPrinterType<T>().printJson(elem, child);
                        json.append(child);
                    }

                }
            };

            template<typename T>
            struct IPrinterType<std::set<T>> {
                inline void operator()(const std::set<T> &s, std::ostream &out, size_t level) {
                    for (const auto &elem:s) {
                        out << tab(level);
                        IPrinterType<T>()(elem, out, level + 1);
                        out << std::endl;
                    }
                }

                inline void printJson(const std::set<T> &s, Json::Value &json) {
                    json = Json::Value(Json::arrayValue);
                    json.resize(s.size());
                    Json::ArrayIndex i = 0;
                    for (const auto &elem:s) {
                        Json::Value child;
                        IPrinterType<T>().printJson(elem, child);
                        json[i++] = child;
                    }

                }
            };

            template<typename T>
            struct IPrinterType<std::unordered_set<T>> {
                inline void operator()(const std::unordered_set<T> &us, std::ostream &out, size_t level) {
                    for (const auto &elem:us) {
                        out << tab(level);
                        IPrinterType<T>()(elem, out, level + 1);
                        out << std::endl;
                    }
                }

                inline void printJson(const std::unordered_set<T> &us, Json::Value &json) {
                    json = Json::Value(Json::arrayValue);
                    json.resize(us.size());
                    Json::ArrayIndex i = 0;
                    for (const auto &elem:us) {
                        Json::Value child;
                        IPrinterType<T>().printJson(elem, child);
                        json[i++] = child;
                    }

                }
            };

            template<typename K, typename V>
            struct IPrinterType<std::map<K, V>> {
                inline void operator()(const std::map<K, V> &m, std::ostream &out, size_t level) {
                    for (const auto &elem:m) {
                        out << tab(level) << "(";
                        IPrinterType<K>()(elem.first, out, level + 1);
                        out << ", ";
                        IPrinterType<K>()(elem.second, out, level + 1);
                        out << ")" << std::endl;
                    }
                }

                inline void printJson(const std::map<K, V> &m, Json::Value &json) {
                    json = Json::Value(Json::objectValue);
                    if(std::is_convertible<K, std::string>()){
                        for (const auto &elem:m) {
                            Json::Value child;
                            IPrinterType<V>().printJson(elem.second, child);
                            json[(std::string&)elem.first] = child;
                        }
                    }else{
                        for (const auto &elem:m) {
                            Json::Value key;
                            Json::Value value;
                            IPrinterType<K>().printJson(elem.first, key);
                            IPrinterType<V>().printJson(elem.second, value);
                            json["key"] = key;
                            json["value"] = value;
                        }
                    }
                }
            };

            template<typename K, typename V>
            struct IPrinterType<std::unordered_map<K, V>> {
                inline void operator()(const std::unordered_map<K, V> &m, std::ostream &out, size_t level) {
                    for (const auto &elem:m) {
                        out << tab(level) << "(";
                        IPrinterType<K>()(elem.first, out, level + 1);
                        out << ", ";
                        IPrinterType<K>()(elem.second, out, level + 1);
                        out << ")" << std::endl;
                    }
                }

                inline void printJson(const std::unordered_map<K, V> &m, Json::Value &json) {
                    json = Json::Value(Json::objectValue);
                    if(std::is_convertible<K, std::string>()){
                        for (const auto &elem:m) {
                            Json::Value child;
                            IPrinterType<V>().printJson(elem.second, child);
                            json[(std::string&)elem.first] = child;
                        }
                    }else{
                        for (const auto &elem:m) {
                            Json::Value key;
                            Json::Value value;
                            IPrinterType<K>().printJson(elem.first, key);
                            IPrinterType<V>().printJson(elem.second, value);
                            json["key"] = key;
                            json["value"] = value;
                        }
                    }
                }
            };


            template<typename T1, typename T2>
            struct IPrinterType<std::pair<T1, T2>> {
                inline void operator()(const std::pair<T1, T2> &p, std::ostream &out, size_t level) {
                    out << tab(level) << "(";
                    IPrinterType<T1>()(p.first, out, level + 1);
                    out << ", ";
                    IPrinterType<T2>()(p.second, out, level + 1);
                    out << ")" << std::endl;
                }

                inline void printJson(const std::pair<T1, T2> &p, Json::Value &json) {
                    json = Json::Value(Json::objectValue);
                    Json::Value second;
                    IPrinterType<T2>().printJson(p.second, second);
                    if(std::is_convertible<T1,std::string>()){
                        json[(std::string&)p.first] = second;
                    }else{
                        Json::Value first;
                        IPrinterType<T1>().printJson(p.first, first);
                        json["first"] = first;
                        json["second"] = second;
                    }
                }
            };

            template<typename T>
            struct IPrinterType<T *> {
                inline void operator()(const T *&v, std::ostream &out, size_t level) {
                    IPrinterType<T *>()(*v, out, level);
                }

                inline void printJson(const T *&v, Json::Value &json) {
                    IPrinterType<T *>().printJson(*v, json);
                }
            };

            template<typename T>
            struct IPrinterType<std::shared_ptr<T>> {
                inline void operator()(const std::shared_ptr<T> &v, std::ostream &out, size_t level) {
                    IPrinterType<T *>()(*v, out, level);
                }

                inline void printJson(const std::shared_ptr<T> &v, Json::Value &json) {
                    IPrinterType<T *>().printJson(*v, json);
                }
            };

            template<typename T>
            class IPrinter {
            public:
                virtual void operator()(const T &b, std::ostream &out) {
                    printer(b, out, 0);
                }

                virtual void printJson(const T &b, Json::Value &json) {
                    printer.printJson(b, json);
                }

            private:
                IPrinterType<T> printer;
            };
        }
    }
}

#endif
