

#ifndef IGNIS_ITERABLE_H
#define IGNIS_ITERABLE_H

#include "IReadIterator.h"

namespace ignis {
    namespace executor {
        namespace api {

            template<typename T>
            class Iterable {
            private:

                template<typename C>
                class IterableIterator : public IReadIterator<T>{
                public:
                    IterableIterator(const C &c) : collection(c), e(collection.begin()) {}

                    IterableIterator(C &&c) : collection(c), e(collection.begin()) {}

                    virtual T &next() override {
                        T& value = *e;
                        e++;
                        return value;
                    }

                    virtual bool hasNext() override {
                        return e != collection.end();
                    }

                    virtual ~IterableIterator() override {}

                private:
                    C collection;
                    typename C::iterator e;
                };

                std::shared_ptr<IReadIterator<T>> iterator;

                Iterable(const std::shared_ptr<IReadIterator<T>> &iterator) : iterator(iterator) {}

            public:

                template<typename C>
                static Iterable<T> fromCollection(const C& c){
                    return Iterable<T>(std::make_shared<IterableIterator<C>>(c));
                }

                template<typename C>
                static Iterable<T> fromCollection(C&& c){
                    return Iterable<T>(std::make_shared<IterableIterator<typename std::remove_reference<C>::type>>(c));
                }

                std::shared_ptr<IReadIterator<T>> readIterator(){
                    return iterator;
                }

            };
        }
    }
}

#endif
