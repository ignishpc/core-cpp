
#ifndef IGNIS_ICLASSMANAGER_H
#define IGNIS_ICLASSMANAGER_H

#include <string>
#include <utility>
#include <vector>
#include "../RTTInfo.h"
#include "ITypeHandle.h"

namespace ignis {
    namespace data {
        namespace serialization {

            template<typename T>
            struct IClassManagerType{

                virtual std::string getName(){
                    return RTTInfo(typeid(T)).getStandardName();
                }

                virtual std::shared_ptr<ITypeHandle<T>> getTypeHandle(){
                    return std::make_shared<ITypeHandle<T>>();
                }

                template<typename C>
                IClassManagerType<C> &as(){
                    return *(IClassManagerType<C>*)this;
                }
            };

            template<typename T>
            struct IClassManagerType<std::vector<T>>{

                virtual std::string getName(){
                    return "vector";
                }

                virtual std::shared_ptr<ITypeHandle<std::vector<T>>> getTypeHandle(){
                    return std::make_shared<ITypeHandle<std::vector<T>>>();
                }

                virtual std::shared_ptr<IClassManagerType<T>> getElemClassManager(){
                    return std::make_shared<IClassManagerType<T>>();
                }

                virtual size_t size(std::vector<T> &v) {
                    v.size();
                }

                virtual void resize(std::vector<T> &v, size_t size) {
                    v.resize(size);
                }

                virtual T &get(std::vector<T> &v, size_t pos) {
                    return v[pos];
                }

                virtual void push(std::vector<T> &v, T &elem) {
                    v.push_back(elem);
                }

                virtual void push(std::vector<T> &v, T &&elem) {
                    v.push_back(elem);
                }

            };

            template<typename T1, typename T2>
            struct IClassManagerType<std::pair<T1,T2>>{

                virtual std::string getName(){
                    return "pair";
                }

                virtual std::shared_ptr<ITypeHandle<std::pair<T1,T2>>> getTypeHandle(){
                    return std::make_shared<ITypeHandle<std::pair<T1,T2>>>();
                }

                virtual std::shared_ptr<IClassManagerType<T1>> getFirstClassManager(){
                    return std::make_shared<IClassManagerType<T1>>();
                }

                virtual std::shared_ptr<IClassManagerType<T2>> getSecondClassManager(){
                    return std::make_shared<IClassManagerType<T2>>();
                }

            };

            template<typename T>
            class IClassManager {
            public:

                virtual std::shared_ptr<IClassManagerType<T>> getClassManagerType(){
                    return std::make_shared<IClassManagerType<T>>();
                }
            };

        }
    }
}

#endif
