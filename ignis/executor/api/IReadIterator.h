
#ifndef IGNIS_IITERATOR_H
#define IGNIS_IITERATOR_H

namespace ignis {
    namespace executor {
        namespace api {
                template<typename T>
                class IReadIterator {
                public:

                    IReadIterator(T &(*next_f)(), bool (*hash_next_f)()) : next_f(next_f), hash_next_f(hash_next_f) {}

                    T &next() {
                        return (*next_f)();
                    }

                    bool hashNext() {
                        return (*hash_next_f)();
                    }

                private:

                    T &(*next_f)();

                    bool (*hash_next_f)();
                };
        }
    }
}


#endif
