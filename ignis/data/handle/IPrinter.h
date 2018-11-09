
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
#include<jsoncpp/json/writer.h>
#include "../../exceptions/ILogicError.h"
#include "../RTTInfo.h"

namespace ignis {
    namespace data {
        namespace handle {

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

            template<typename _Tp, typename _Alloc>
            struct IPrinterType<std::vector<_Tp, _Alloc>> {
                inline void operator()(const std::vector<_Tp, _Alloc> &v, std::ostream &out, size_t level) {
                    auto begin = v.begin();
                    auto end = v.end();
                    if (begin != end) {
                        IPrinterType<_Tp>()(*begin, out, level + 1);
                        begin++;
                    }

                    while (begin != end) {
                        out << std::endl;
                        IPrinterType<_Tp>()(*begin, out, level + 1);
                        begin++;
                    }
                }

                inline void printJson(const std::vector<_Tp, _Alloc> &v, Json::Value &json) {
                    json = Json::Value(Json::arrayValue);
                    json.resize(v.size());
                    Json::ArrayIndex i = 0;
                    for (const auto &elem:v) {
                        Json::Value child;
                        IPrinterType<_Tp>().printJson(elem, child);
                        json[i++] = child;
                    }

                }
            };

            template<typename _Tp, typename _Alloc>
            struct IPrinterType<std::list<_Tp, _Alloc>> {
                inline void operator()(const std::list<_Tp, _Alloc> &l, std::ostream &out, size_t level) {
                    auto begin = l.begin();
                    auto end = l.end();
                    if (begin != end) {
                        IPrinterType<_Tp>()(*begin, out, level + 1);
                        begin++;
                    }

                    while (begin != end) {
                        out << std::endl;
                        IPrinterType<_Tp>()(*begin, out, level + 1);
                        begin++;
                    }
                }

                inline void printJson(const std::list<_Tp, _Alloc> &l, Json::Value &json) {
                    json = Json::Value(Json::arrayValue);
                    json.resize(l.size());
                    Json::ArrayIndex i = 0;
                    for (const auto &elem:l) {
                        Json::Value child;
                        IPrinterType<_Tp>().printJson(elem, child);
                        json[i++] = child;
                    }

                }
            };

            template<typename _Tp, typename _Alloc>
            struct IPrinterType<std::forward_list<_Tp, _Alloc>> {
                inline void operator()(const std::forward_list<_Tp, _Alloc> &fl, std::ostream &out, size_t level) {
                    auto begin = fl.begin();
                    auto end = fl.end();
                    if (begin != end) {
                        IPrinterType<_Tp>()(*begin, out, level + 1);
                        begin++;
                    }

                    while (begin != end) {
                        out << std::endl;
                        IPrinterType<_Tp>()(*begin, out, level + 1);
                        begin++;
                    }
                }

                inline void printJson(const std::forward_list<_Tp, _Alloc> &fl, Json::Value &json) {
                    json = Json::Value(Json::arrayValue);
                    for (const auto &elem:fl) {
                        Json::Value child;
                        IPrinterType<_Tp>().printJson(elem, child);
                        json.append(child);
                    }

                }
            };

            template<typename _Key, typename _Compare>
            struct IPrinterType<std::set<_Key, _Compare>> {
                inline void operator()(const std::set<_Key, _Compare> &s, std::ostream &out, size_t level) {
                    auto begin = s.begin();
                    auto end = s.end();
                    if (begin != end) {
                        IPrinterType<_Key>()(*begin, out, level + 1);
                        begin++;
                    }

                    while (begin != end) {
                        out << std::endl;
                        IPrinterType<_Key>()(*begin, out, level + 1);
                        begin++;
                    }
                }

                inline void printJson(const std::set<_Key, _Compare> &s, Json::Value &json) {
                    json = Json::Value(Json::arrayValue);
                    json.resize(s.size());
                    Json::ArrayIndex i = 0;
                    for (const auto &elem:s) {
                        Json::Value child;
                        IPrinterType<_Key>().printJson(elem, child);
                        json[i++] = child;
                    }

                }
            };

            template<typename _Value, typename _Hash, typename _Pred, typename _Alloc>
            struct IPrinterType<std::unordered_set<_Value, _Hash, _Pred, _Alloc>> {
                inline void operator()(const std::unordered_set<_Value> &us, std::ostream &out, size_t level) {
                    auto begin = us.begin();
                    auto end = us.end();
                    if (begin != end) {
                        IPrinterType<_Value>()(*begin, out, level + 1);
                        begin++;
                    }

                    while (begin != end) {
                        out << std::endl;
                        IPrinterType<_Value>()(*begin, out, level + 1);
                        begin++;
                    }
                }

                inline void printJson(const std::unordered_set<_Value, _Hash, _Pred, _Alloc> &us, Json::Value &json) {
                    json = Json::Value(Json::arrayValue);
                    json.resize(us.size());
                    Json::ArrayIndex i = 0;
                    for (const auto &elem:us) {
                        Json::Value child;
                        IPrinterType<_Value>().printJson(elem, child);
                        json[i++] = child;
                    }

                }
            };

            template<typename _Key, typename _Tp, typename _Compare, typename _Alloc>
            struct IPrinterType<std::map<_Key, _Tp, _Compare, _Alloc>> {
                inline void
                operator()(const std::map<_Key, _Tp, _Compare, _Alloc> &m, std::ostream &out, size_t level) {
                    auto begin = m.begin();
                    auto end = m.end();
                    if (begin != end) {
                        IPrinterType<std::pair<_Key,_Tp>>()(*begin, out, level);
                        begin++;
                    }

                    while (begin != end) {
                        out << std::endl;
                        IPrinterType<std::pair<_Key,_Tp>>()(*begin, out, level);
                        begin++;
                    }
                }

                inline void printJson(const std::map<_Key, _Tp, _Compare, _Alloc> &m, Json::Value &json) {
                    json = Json::Value(Json::objectValue);
                    if (std::is_convertible<_Key, std::string>()) {
                        for (const auto &elem:m) {
                            Json::Value child;
                            IPrinterType<_Tp>().printJson(elem.second, child);
                            json[(std::string &) elem.first] = child;
                        }
                    } else {
                        for (const auto &elem:m) {
                            Json::Value key;
                            Json::Value value;
                            IPrinterType<_Key>().printJson(elem.first, key);
                            IPrinterType<_Tp>().printJson(elem.second, value);
                            json["key"] = key;
                            json["value"] = value;
                        }
                    }
                }
            };

            template<typename _Key, typename _Tp, typename _Hash, typename _Pred, typename _Alloc>
            struct IPrinterType<std::unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc>> {
                inline void operator()(const std::unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc> &um, std::ostream &out,
                                       size_t level) {
                    auto begin = um.begin();
                    auto end = um.end();
                    if (begin != end) {
                        IPrinterType<std::pair<_Key,_Tp>>()(*begin, out, level);
                        begin++;
                    }

                    while (begin != end) {
                        out << std::endl;
                        IPrinterType<std::pair<_Key,_Tp>>()(*begin, out, level);
                        begin++;
                    }
                }

                inline void
                printJson(const std::unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc> &um, Json::Value &json) {
                    json = Json::Value(Json::objectValue);
                    if (std::is_convertible<_Key, std::string>()) {
                        for (const auto &elem:um) {
                            Json::Value child;
                            IPrinterType<_Tp>().printJson(elem.second, child);
                            json[(std::string &) elem.first] = child;
                        }
                    } else {
                        for (const auto &elem:um) {
                            Json::Value key;
                            Json::Value value;
                            IPrinterType<_Key>().printJson(elem.first, key);
                            IPrinterType<_Tp>().printJson(elem.second, value);
                            json["key"] = key;
                            json["value"] = value;
                        }
                    }
                }
            };


            template<typename _T1, typename _T2>
            struct IPrinterType<std::pair<_T1, _T2>> {
                inline void operator()(const std::pair<_T1, _T2> &p, std::ostream &out, size_t level) {
                    out << tab(level) << "(";
                    IPrinterType<_T1>()(p.first, out, level + 1);
                    out << ", ";
                    IPrinterType<_T2>()(p.second, out, level + 1);
                    out << ")";
                }

                inline void printJson(const std::pair<_T1, _T2> &p, Json::Value &json) {
                    json = Json::Value(Json::objectValue);
                    Json::Value second;
                    IPrinterType<_T2>().printJson(p.second, second);
                    if (std::is_convertible<_T1, std::string>()) {
                        json[(std::string &) p.first] = second;
                    } else {
                        Json::Value first;
                        IPrinterType<_T1>().printJson(p.first, first);
                        json["first"] = first;
                        json["second"] = second;
                    }
                }
            };

            template<typename _Tp>
            struct IPrinterType<_Tp *> {
                inline void operator()(const _Tp *&v, std::ostream &out, size_t level) {
                    IPrinterType<_Tp *>()(*v, out, level);
                }

                inline void printJson(const _Tp *&v, Json::Value &json) {
                    IPrinterType<_Tp *>().printJson(*v, json);
                }
            };

            template<typename _Tp>
            struct IPrinterType<std::shared_ptr<_Tp>> {
                inline void operator()(const std::shared_ptr<_Tp> &v, std::ostream &out, size_t level) {
                    IPrinterType<_Tp *>()(*v, out, level);
                }

                inline void printJson(const std::shared_ptr<_Tp> &v, Json::Value &json) {
                    IPrinterType<_Tp *>().printJson(*v, json);
                }
            };

            template<typename T>
            class IPrinter {
            public:
                virtual void operator()(const T &b, std::ostream &out) {
                    printer(b, out, 0);
                }

                virtual void printJson(const T &b, std::ostream &out) {
                    Json::Value root;
                    printer.printJson(b, root);
                    out << root;
                }

            private:
                IPrinterType<T> printer;
            };
        }
    }
}

#endif
