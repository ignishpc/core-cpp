

#ifndef IGNIS_ITERABLE_H
#define IGNIS_ITERABLE_H

#include "IReadIterator.h"

namespace ignis {
    namespace executor {
        namespace api {
            namespace function {
                template<typename C>
                class Iterable {
                public:

                    typedef typename C::value_type value_type;
                    typedef typename C::iterator iterator;

                private:
                    class IterableIterator : api::IReadIterator<value_type> {
                    public:
                        IterableIterator(C &c) : collection(c), e(NULL) {}

                        virtual value_type &next() override {
                            e++;
                            return (value_type &)*e;
                        }

                        virtual bool hashNext() override {
                            return e == collection.end();
                        }

                        virtual ~IterableIterator() override {

                        }

                    private:
                        C &collection;
                        iterator e;
                    };

                    C collection;

                public:

                    Iterable(){}

                    Iterable(const C&& c ) : collection(c){}

                    virtual C &getCollection() {
                        return collection;
                    }

                    virtual std::shared_ptr<api::IReadIterator<value_type>> readIterator() {
                        auto it = std::make_shared<IterableIterator>(collection);
                        return (std::shared_ptr<api::IReadIterator<value_type>>&)it;
                    }

                };
            }
        }
    }
}

namespace std {
    template<class T>
    struct hash<ignis::executor::api::function::Iterable<T>> {
        std::size_t operator()(ignis::executor::api::function::Iterable<T> const &it) const noexcept {

        }
    };
}

#endif
