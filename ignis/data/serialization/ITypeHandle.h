
#ifndef IGNIS_ITYPEHANDLE_H
#define IGNIS_ITYPEHANDLE_H

#include <memory>
#include <vector>
#include "IPrinter.h"
#include "IReader.h"
#include "IWriter.h"
#include "IDeleter.h"


namespace ignis {
    namespace data {
        namespace serialization {

            template<typename T>
            class ITypeHandleBase {
            public:
                virtual std::shared_ptr<IPrinter<T>> printer() { return std::make_shared<IPrinter<T>>(); }

                virtual std::shared_ptr<IReader<T>> reader() { return std::make_shared<IReader<T>>(); }

                virtual std::shared_ptr<IWriter<T>> writer() { return std::make_shared<IWriter<T>>(); }

                virtual std::shared_ptr<IDeleter<T>> deleter() { return std::make_shared<IDeleter<T>>(); }

                virtual const std::type_info &info() { return typeid(T); }
            };

            template<typename T>
            class IColectionHandle : public ITypeHandleBase<std::vector<T>> {
            public:

                virtual size_t getSize(std::vector<T> &v) {
                    return v.size();
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

            template<typename T>
            class ITypeHandle : public ITypeHandleBase<T> {
            public:

                virtual std::shared_ptr<IColectionHandle<T>> collectionHandle() {
                    return std::shared_ptr<IColectionHandle<T>>();
                }

            };
        }
    }
}
#endif
