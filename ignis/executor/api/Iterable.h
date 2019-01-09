

#ifndef IGNIS_ITERABLE_H
#define IGNIS_ITERABLE_H

#include "IReadIterator.h"

namespace ignis {
    namespace executor {
        namespace api {

            template<typename T>
            class Iterable {
            private:

                class ValueIterator : public IReadIterator<T>{
                public:
                    ValueIterator(const T &v) : value(v),read(false){}

                    ValueIterator(T &&v) : value(v),read(false){}

                    virtual T &next() override {
                        read = true;
                        return value;
                    }

                    virtual bool hasNext() override {
                        return !read;
                    }

                    virtual ~ValueIterator() override {}

                private:
                    T value;
                    bool read;
                };

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

                static Iterable<T> fromValue(const T &v){
                    return Iterable<T>(std::make_shared<ValueIterator>(v));
                }

                static Iterable<T> fromValue(T &&v){
                    return Iterable<T>(std::make_shared<ValueIterator>(v));
                }

                std::shared_ptr<IReadIterator<T>> readIterator(){
                    return iterator;
                }

            };
        }
    }
}

#endif
