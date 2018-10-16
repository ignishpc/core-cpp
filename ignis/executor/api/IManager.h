
#ifndef IGNIS_IMANAGER_H
#define IGNIS_IMANAGER_H

#include <memory>
#include <utility>
#include <type_traits>
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

                IBasicManager() :
                        type_info(typeid(T)),
                        operator_var(std::make_shared<data::handle::IOperator<T>>()),
                        deleter_var(std::make_shared<data::handle::IDeleter<T>>()),
                        writer_var(std::make_shared<data::handle::IWriter<T>>()),
                        reader_var(std::make_shared<data::handle::IReader<T>>()),
                        printer_var(std::make_shared<data::handle::IPrinter<T>>()) {}

                virtual std::shared_ptr<data::handle::IPrinter<T>> printer() {
                    return printer_var;
                }

                virtual std::shared_ptr<data::handle::IReader<T>> reader() {
                    return reader_var;
                }

                virtual std::shared_ptr<data::handle::IWriter<T>> writer() {
                    return writer_var;
                }

                virtual std::shared_ptr<data::handle::IDeleter<T>> deleter() {
                    return deleter_var;
                }

                virtual std::shared_ptr<data::handle::IOperator<T>> _operator() {
                    return operator_var;
                }

                virtual const std::type_info &info() {
                    return type_info;
                }

            private:
                const std::type_info &type_info;
                std::shared_ptr<data::handle::IOperator<T>> operator_var;
                std::shared_ptr<data::handle::IDeleter<T>> deleter_var;
                std::shared_ptr<data::handle::IWriter<T>> writer_var;
                std::shared_ptr<data::handle::IReader<T>> reader_var;
                std::shared_ptr<data::handle::IPrinter<T>> printer_var;

            };

            template<typename T>
            class ICollectionManager : public IBasicManager<std::vector<T>> {
            public:

                typedef std::vector<T> Class;
                typedef typename std::conditional<std::is_same<T, bool>::value, T, T &>::type Ref;

                virtual bool isBool(){
                    return std::is_same<T, bool>::value;
                }

                virtual size_t size(Class &v) {
                    return v.size();
                }

                virtual void clear(Class &v) {
                    v.clear();
                }

                virtual void resize(Class &v, size_t size) {
                    v.resize(size);
                }

                virtual Ref get(Class &v, size_t pos) {
                    return v[pos];
                }

                virtual void push(Class &v, T &elem) {
                    v.push_back(elem);
                }

                virtual void push(Class &v, T &&elem) {
                    v.push_back((T&&)elem);
                }

            };

            template<typename T>
            class IManager : public IBasicManager<T> {
            public:

                IManager() : collection_manager(std::make_shared<ICollectionManager<T>>()) {}

                virtual std::shared_ptr<ICollectionManager<T>> collectionManager() {
                    return collection_manager;
                }

            private:
                std::shared_ptr<ICollectionManager<T>> collection_manager;
            };

            template<typename T1, typename T2>
            class IPairManager : public IManager<std::pair<T1, T2>> {
            public:

                typedef std::pair<T1, T2> Class;

                IPairManager() :
                        first_manager(std::make_shared<IManager<T1>>()),
                        second_manager(std::make_shared<IManager<T2>>()) {}

                virtual T1 &first(Class &p) {
                    return p.first;
                }

                virtual T2 &second(Class &p) {
                    return p.second;
                }

                virtual std::shared_ptr<IManager<T1>> firstManager() {
                    return first_manager;
                }

                virtual std::shared_ptr<IManager<T2>> secondManager() {
                    return second_manager;
                }

            private:
                std::shared_ptr<IManager<T1>> first_manager;
                std::shared_ptr<IManager<T2>> second_manager;

            };
        }
    }
}

#endif
