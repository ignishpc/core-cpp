
#ifndef IGNIS_IITERATOR_H
#define IGNIS_IITERATOR_H

namespace ignis {
    namespace executor {
        namespace api {
            template<typename T>
            class IReadIterator {
            public:

                virtual T *next() = 0;

                virtual bool hashNext() = 0;
            };
        }
    }
}


#endif
