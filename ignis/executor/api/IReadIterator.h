
#ifndef IGNIS_IREADITERATOR_H
#define IGNIS_IREADITERATOR_H

#include <memory>

namespace ignis {
    namespace executor {
        namespace api {
            template<typename Tp>
            class IReadIterator {
            public:
                typedef IReadIterator<Tp> _IReadIterator_type;

                virtual Tp &next() = 0;

                virtual std::shared_ptr<Tp> nextShared() = 0;

                virtual bool hasNext() = 0;
            };
        }
    }
}


#endif
