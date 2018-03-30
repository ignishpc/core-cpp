
#ifndef IGNIS_ITYPEINFO_H
#define IGNIS_ITYPEINFO_H

#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <forward_list>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <utility>

#include "RTTInfo.h"

namespace ignis {
    namespace data {
        class ITypeInfo {
        private:
            size_t id;
            std::string name_id;
            std::vector<ITypeInfo> args_v;

        protected:

            ITypeInfo(size_t id, std::string &name_id) : id(id),
                                                         name_id(name_id) {
                args_v.shrink_to_fit();
            }

            ITypeInfo() {}

            template<template<typename...> typename T>
            static ITypeInfo registerT(std::string name) {
                auto p = &registerT<T>;
                return ITypeInfo((size_t) p, name);
            }

            template<typename T>
            static ITypeInfo registerC(std::string name = RTTInfo(typeid(T)).getStandardName()) {
                auto p = &registerC<T>;
                return ITypeInfo((size_t) p, name);
            }

            template<typename T>
            static ITypeInfo arg(ITypeInfo &&i) {
                i.args_v.push_back(info<>((T *) NULL));
                return i;
            }

            void putArg(ITypeInfo &i) {
                args_v.push_back(i);
            }

        public:

            static ITypeInfo voidType(){
                auto p = &voidType;
                std::string name = "void";
                return ITypeInfo((size_t) p, name);
            }

            static ITypeInfo pointerType(){
                auto p = &pointerType;
                std::string name = "*";
                return ITypeInfo((size_t) p, name);
            }

            template<typename T>
            static ITypeInfo info(std::vector<T> *t) {
                return arg<T>(registerT<std::vector>("vector"));
            };

            template<typename T>
            static ITypeInfo info(std::list<T> *t) {
                return arg<T>(registerT<std::list>("list"));
            };

            template<typename T>
            static ITypeInfo info(std::forward_list<T> *t) {
                return arg<T>(registerT<std::forward_list>("forward_list"));
            };

            template<typename T>
            static ITypeInfo info(std::set<T> *t) {
                return arg<T>(registerT<std::set>("set"));
            };

            template<typename T>
            static ITypeInfo info(std::unordered_set<T> *t) {
                return arg<T>(registerT<std::unordered_set>("unordered_set"));
            };

            template<typename K, typename V>
            static ITypeInfo info(std::map<K, V> *t) {
                return arg<V>(arg<K>(registerT<std::map>("map")));
            };

            template<typename K, typename V>
            static ITypeInfo info(std::unordered_map<K, V> *t) {
                return arg<V>(arg<K>(registerT<std::unordered_map>("unordered_map")));
            };

            template<typename T1, typename T2>
            static ITypeInfo info(std::pair<T1, T2> *t) {
                return arg<T2>(arg<T1>(registerT<std::pair>("pair")));
            };

            template<typename T>
            static ITypeInfo info(T **t) {
                return arg<T>(pointerType());
            }

            template<typename T>
            static ITypeInfo info(T *t) {
                return registerC<T>();
            }

            template<typename T>
            static ITypeInfo getInfo(T *t = NULL) {
                return info<>(t);
            };

            ITypeInfo operator[](size_t i) {
                return args_v[i];
            };

            size_t templates() {
                return args_v.size();
            }

            template<template<typename...> typename T>
            bool is() {
                return id == registerT<T>("").id;
            }

            bool operator==(ITypeInfo &other) {
                if (other.id != id || other.args_v.size() != args_v.size()) {
                    return false;
                }
                for (int i = 0; i < args_v.size(); i++) {
                    if (other.args_v[i] != args_v[i]) {
                        return false;
                    }
                }
                return true;
            }

            bool operator!=(ITypeInfo &other) {
                return !((*this) == other);
            }

            size_t getId(){
                return id;
            }

            std::string nameId() {
                return name_id;
            }

            std::string name() {
                std::stringstream str;
                str << name_id;
                if (!args_v.empty()) {
                    str << "<";
                    for (int i = 0; args_v.size() > i + 1; i++) {
                        str << args_v[i].name() << ",";
                    }
                    str << args_v[args_v.size() - 1].name() << ">";
                }
                return str.str();
            }
        };

    }
}
#endif
