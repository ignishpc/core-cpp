
#ifndef IGNIS_IREADITERATOR_H
#define IGNIS_IREADITERATOR_H

namespace ignis {
    namespace executor {
        namespace api {
            template<typename T>
            class IReadIterator {
            public:

                virtual T &next() = 0;

                virtual bool hasNext() = 0;

                virtual ~IReadIterator() {};
            };
        }
    }
}


#endif
