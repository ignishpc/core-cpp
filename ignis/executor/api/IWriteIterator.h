
#ifndef IGNIS_IWRITEITERATOR_H
#define IGNIS_IWRITEITERATOR_H

namespace ignis {
    namespace executor {
        namespace api {
            template<typename T>
            class IWriteIterator {
            public:

                virtual void write(T &obj) = 0;

                virtual void write(T &&obj) = 0;

                virtual ~IWriteIterator(){};
            };
        }
    }
}
#endif
