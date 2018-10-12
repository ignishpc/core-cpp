
#ifndef IGNIS_IMANAGER_H
#define IGNIS_IMANAGER_H

#include <memory>
#include <vector>
#include <utility>
#include "../../data/handle/IPrinter.h"
#include "../../data/handle/IReader.h"
#include "../../data/handle/IWriter.h"
#include "../../data/handle/IDeleter.h"
#include "../../data/handle/IOperators.h"

namespace ignis {
    namespace executor {
        namespace api {

            template<typename T>
            class IBasicManager {
            public:
                virtual std::shared_ptr<data::handle::IPrinter<T>> printer() {
                    return std::make_shared<data::handle::IPrinter<T>>();
                }

                virtual std::shared_ptr<data::handle::IReader<T>> reader() {
                    return std::make_shared<data::handle::IReader<T>>();
                }

                virtual std::shared_ptr<data::handle::IWriter<T>> writer() {
                    return std::make_shared<data::handle::IWriter<T>>();
                }

                virtual std::shared_ptr<data::handle::IDeleter<T>> deleter() {
                    return std::make_shared<data::handle::IDeleter<T>>();
                }

                virtual std::shared_ptr<data::handle::IOperator<T>> _operator(){
                    return std::make_shared<data::handle::IOperator<T>>();
                }

                virtual const std::type_info &info() {
                    return typeid(T);
                }

            };

            template<typename T>
            class ICollectionManager : public IBasicManager<std::vector<T>> {
            public:

                typedef  std::vector<T> Class;

                virtual size_t size(Class &v) {
                    return v.size();
                }

                virtual void clear(Class &v) {
                    v.clear();
                }

                virtual void resize(Class &v, size_t size) {
                    v.resize(size);
                }

                virtual T &get(Class &v, size_t pos) {
                    return v[pos];
                }

                virtual void push(Class &v, T &elem) {
                    v.push_back(elem);
                }

                virtual void push(Class &v, T &&elem) {
                    v.push_back(elem);
                }

            };

            template<typename T>
            class IManager : public IBasicManager<T> {
            public:
                virtual std::shared_ptr<ICollectionManager<T>> collectionManager() {
                    return std::make_shared<ICollectionManager<T>>();
                }

                virtual std::shared_ptr<IManager<T>> shared() const{
                    return std::make_shared<IManager<T>>();
                }

            };

            template<typename T1, typename T2>
            class IPairManager : public IManager<std::pair<T1, T2>> {
            public:

                typedef  std::pair<T1, T2> Class;

                virtual T1 &first(Class &p) {
                    return p.first;
                }

                virtual T2 &second(Class &p) {
                    return p.second;
                }



                virtual std::shared_ptr<IManager<T1>> firstManager() const{
                    return std::make_shared<IManager<T1>>();
                }

                virtual std::shared_ptr<IManager<T2>> secondManager() const{
                    return std::make_shared<IManager<T2>>();
                }

                virtual std::shared_ptr<IManager<Class>> shared() const{
                    return std::make_shared<IPairManager<T1,T2>>();
                }

            };
        }
    }
}

#endif
