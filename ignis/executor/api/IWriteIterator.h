
#ifndef IGNIS_IWRITEITERATOR_H
#define IGNIS_IWRITEITERATOR_H

namespace ignis {
    namespace executor {
        namespace api {
            template<typename T>
            class IWriteIterator {
            public:

                IWriteIterator(void (*write_f)(T &&)) : write_f(write_f) {}

                void write(T &obj) {
                    (*write_f)(T(obj));
                }

                void write(T &&obj) {
                    (*write_f)(obj);
                }

            private:

                void (*write_f)(T &&);
            };
        }
    }
}
#endif
