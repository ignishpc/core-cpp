
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

                virtual void copy(T &source, T &target) {
                    target = source;
                }

                virtual void move(T &source, T &target) {
                    target = std::move(source);
                }

                virtual T *_new() {
                    return new T();
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
            private:
                class Ref {
                public:
                    template<class Q = T>
                    inline static typename std::enable_if<std::is_same<Q, bool>::value, bool &>::type
                    get(Class &v, size_t pos) {
                        static bool v_true = true;
                        static bool v_false = false;
                        if (v[pos]) {
                            return v_true;
                        }
                        return v_false;
                    }

                    template<class Q = T>
                    inline static typename std::enable_if<!std::is_same<Q, bool>::value, T &>::type
                    get(Class &v, size_t pos) {
                        return v[pos];
                    }
                };

            public:

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
                    return Ref::get(v, pos);
                }

                virtual void push(Class &v, T &elem) {
                    v.push_back(elem);
                }

                virtual void push(Class &v, T &&elem) {
                    v.push_back((T &&) elem);
                }

            };

            template<typename T>
            class IBaseManager : public IBasicManager<T> {
            public:
                IBaseManager() : collection_manager(std::make_shared<ICollectionManager<T>>()) {}

                virtual std::shared_ptr<ICollectionManager<T>> collectionManager() {
                    return collection_manager;
                }

            private:
                std::shared_ptr<ICollectionManager<T>> collection_manager;
            };

            template<typename T>
            class IManager : public IBaseManager<T>{};

            template<typename T1, typename T2>
            class IManager<std::pair<T1, T2>> : public IBaseManager<std::pair<T1, T2>> {
            public:

                typedef std::pair<T1, T2> Class;

                IManager() :
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

            template<typename T1, typename T2>
            class IPairManager : public IManager<std::pair<T1, T2>>{};

        }
    }
}

#endif
