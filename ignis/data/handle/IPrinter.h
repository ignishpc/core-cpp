
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
#include <rapidjson/prettywriter.h>
#include <rapidjson/ostreamwrapper.h>
#include "../../exceptions/ILogicError.h"
#include "../RTTInfo.h"

namespace ignis {
    namespace data {
        namespace handle {

            std::string inline tab(size_t n) { return std::string(n, '\t'); }

            typedef rapidjson::Writer<rapidjson::OStreamWrapper> JsonStream;

            template<typename T>
            struct IPrinterType {
                virtual void operator()(const T &b, std::ostream &out, size_t level) {
                    throw exceptions::ILogicError(
                            "IPrinterType not implemented for " + RTTInfo(typeid(T)).getStandardName());
                }

                virtual void printJson(const T &b, JsonStream &out) {
                    throw exceptions::ILogicError(
                            "IPrinterType json not implemented for " + RTTInfo(typeid(T)).getStandardName());
                }
            };

            template<>
            struct IPrinterType<bool> {
                inline void operator()(const bool &b, std::ostream &out, size_t level) {
                    out << b;
                }

                inline void printJson(const bool &b, JsonStream &out) {
                    out.Bool(b);
                }
            };

            template<>
            struct IPrinterType<int8_t> {
                inline void operator()(const int8_t &i, std::ostream &out, size_t level) {
                    out << i;
                }

                inline void printJson(const int8_t &i, JsonStream &out) {
                    out.Int(i);
                }
            };

            template<>
            struct IPrinterType<uint8_t> {
                inline void operator()(const uint8_t &i, std::ostream &out, size_t level) {
                    out << i;
                }

                inline void printJson(const uint8_t &i, JsonStream &out) {
                    out.Uint(i);
                }
            };

            template<>
            struct IPrinterType<int16_t> {
                inline void operator()(const int16_t &i, std::ostream &out, size_t level) {
                    out << i;
                }

                inline void printJson(const int16_t &i, JsonStream &out) {
                    out.Int(i);
                }
            };

            template<>
            struct IPrinterType<uint16_t> {
                inline void operator()(const uint16_t &i, std::ostream &out, size_t level) {
                    out << i;
                }

                inline void printJson(const uint16_t &i, JsonStream &out) {
                    out.Uint(i);
                }
            };

            template<>
            struct IPrinterType<int32_t> {
                inline void operator()(const int32_t &i, std::ostream &out, size_t level) {
                    out << i;
                }

                inline void printJson(const int32_t &i, JsonStream &out) {
                    out.Int(i);
                }
            };

            template<>
            struct IPrinterType<uint32_t> {
                inline void operator()(const uint32_t &i, std::ostream &out, size_t level) {
                    out << i;
                }

                inline void printJson(const uint32_t &i, JsonStream &out) {
                    out.Uint(i);
                }
            };

            template<>
            struct IPrinterType<int64_t> {
                inline void operator()(const int64_t &i, std::ostream &out, size_t level) {
                    out << i;
                }

                inline void printJson(const int64_t &i, JsonStream &out) {
                    out.Int64(i);
                }
            };

            template<>
            struct IPrinterType<uint64_t> {
                inline void operator()(const uint64_t &i, std::ostream &out, size_t level) {
                    out << i;
                }

                inline void printJson(const uint64_t &i, JsonStream &out) {
                    out.Uint64(i);
                }
            };

            template<>
            struct IPrinterType<float> {
                inline void operator()(const float &d, std::ostream &out, size_t level) {
                    out << d;
                }

                inline void printJson(const float &d, JsonStream &out) {
                    out.Double(d);
                }
            };

            template<>
            struct IPrinterType<double> {
                inline void operator()(const double &d, std::ostream &out, size_t level) {
                    out << d;
                }

                inline void printJson(const double &d, JsonStream &out) {
                    out.Double(d);
                }
            };

            template<>
            struct IPrinterType<std::string> {
                inline void operator()(const std::string &s, std::ostream &out, size_t level) {
                    out << s;
                }

                inline void printJson(const std::string &s, JsonStream &out) {
                    out.String(s.data());
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

                inline void printJson(const std::vector<_Tp, _Alloc> &v, JsonStream &out) {
                    out.StartArray();
                    for (const auto &elem:v) {
                        IPrinterType<_Tp>().printJson(elem, out);
                    }
                    out.EndArray();
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

                inline void printJson(const std::list<_Tp, _Alloc> &l, JsonStream &out) {
                    out.StartArray();
                    for (const auto &elem:l) {
                        IPrinterType<_Tp>().printJson(elem, out);
                    }
                    out.EndArray();
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

                inline void printJson(const std::forward_list<_Tp, _Alloc> &fl, JsonStream &out) {
                    out.StartArray();
                    for (const auto &elem:fl) {
                        IPrinterType<_Tp>().printJson(elem, out);
                    }
                    out.EndArray();
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

                inline void printJson(const std::set<_Key, _Compare> &s, JsonStream &out) {
                    out.StartArray();
                    for (const auto &elem:s) {
                        IPrinterType<_Key>().printJson(elem, out);
                    }
                    out.EndArray();
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

                inline void printJson(const std::unordered_set<_Value, _Hash, _Pred, _Alloc> &us, JsonStream &out) {
                    out.StartArray();
                    for (const auto &elem:us) {
                        IPrinterType<_Value>().printJson(elem, out);
                    }
                    out.EndArray();
                }
            };

            template<typename _Key, typename _Tp, typename _Compare, typename _Alloc>
            struct IPrinterType<std::map<_Key, _Tp, _Compare, _Alloc>> {
                inline void
                operator()(const std::map<_Key, _Tp, _Compare, _Alloc> &m, std::ostream &out, size_t level) {
                    auto begin = m.begin();
                    auto end = m.end();
                    if (begin != end) {
                        IPrinterType<std::pair<_Key, _Tp>>()(*begin, out, level);
                        begin++;
                    }

                    while (begin != end) {
                        out << std::endl;
                        IPrinterType<std::pair<_Key, _Tp>>()(*begin, out, level);
                        begin++;
                    }
                }

                inline void printJson(const std::map<_Key, _Tp, _Compare, _Alloc> &m, JsonStream &out) {
                    out.StartObject();
                    if (std::is_convertible<_Key, std::string>()) {
                        for (const auto &elem:m) {
                            out.Key(((std::string &) elem.first).data());
                            IPrinterType<_Tp>().printJson(elem.second, out);
                        }
                    } else {
                        for (const auto &elem:m) {
                            out.Key("key");
                            IPrinterType<_Key>().printJson(elem.first, out);
                            out.Key("value");
                            IPrinterType<_Tp>().printJson(elem.second, out);
                        }
                    }
                    out.EndObject();
                }
            };

            template<typename _Key, typename _Tp, typename _Hash, typename _Pred, typename _Alloc>
            struct IPrinterType<std::unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc>> {
                inline void operator()(const std::unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc> &um, std::ostream &out,
                                       size_t level) {
                    auto begin = um.begin();
                    auto end = um.end();
                    if (begin != end) {
                        IPrinterType<std::pair<_Key, _Tp>>()(*begin, out, level);
                        begin++;
                    }

                    while (begin != end) {
                        out << std::endl;
                        IPrinterType<std::pair<_Key, _Tp>>()(*begin, out, level);
                        begin++;
                    }
                }

                inline void
                printJson(const std::unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc> &um, JsonStream &out) {
                    out.StartObject();
                    if (std::is_convertible<_Key, std::string>()) {
                        for (const auto &elem:um) {
                            out.Key(((std::string &) elem.first).data());
                            IPrinterType<_Tp>().printJson(elem.second, out);
                        }
                    } else {
                        for (const auto &elem:um) {
                            out.Key("key");
                            IPrinterType<_Key>().printJson(elem.first, out);
                            out.Key("value");
                            IPrinterType<_Tp>().printJson(elem.second, out);
                        }
                    }
                    out.EndObject();
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

                inline void printJson(const std::pair<_T1, _T2> &p, JsonStream &out) {
                    out.StartObject();
                    if (std::is_convertible<_T1, std::string>()) {
                        out.Key(((std::string &) p.first).data());
                        IPrinterType<_T2>().printJson(p.second, out);

                    } else {
                        out.Key("first");
                        IPrinterType<_T1>().printJson(p.first, out);
                        out.Key("second");
                        IPrinterType<_T2>().printJson(p.second, out);
                    }
                    out.EndObject();
                }
            };

            template<typename _Tp>
            struct IPrinterType<_Tp *> {
                inline void operator()(const _Tp *&v, std::ostream &out, size_t level) {
                    IPrinterType<_Tp *>()(*v, out, level);
                }

                inline void printJson(const _Tp *&v, JsonStream &out) {
                    IPrinterType<_Tp *>().printJson(*v, out);
                }
            };

            template<typename _Tp>
            struct IPrinterType<std::shared_ptr<_Tp>> {
                inline void operator()(const std::shared_ptr<_Tp> &v, std::ostream &out, size_t level) {
                    IPrinterType<_Tp *>()(*v, out, level);
                }

                inline void printJson(const std::shared_ptr<_Tp> &v, JsonStream &out) {
                    IPrinterType<_Tp *>().printJson(*v, out);
                }
            };

            template<typename T>
            class IPrinter {
            public:
                virtual void operator()(const T &b, std::ostream &out) {
                    printer(b, out, 0);
                }

                virtual void printJson(const T &b, std::ostream &out, bool pretty = true) {
                    rapidjson::OStreamWrapper wrapper(out);
                    if (pretty) {
                        rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(wrapper);
                        printer.printJson(b, (JsonStream&)writer);

                    } else {
                        JsonStream writer(wrapper);
                        printer.printJson(b, writer);
                    }
                }

            private:
                IPrinterType<T> printer;
            };
        }
    }
}

#endif
