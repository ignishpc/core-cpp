
#ifndef IGNIS_IWRITEITERATOR_H
#define IGNIS_IWRITEITERATOR_H

namespace ignis {
    namespace executor {
        namespace api {
            template<typename Tp>
            class IWriteIterator {
            public:
                typedef IReadIterator<Tp> _IWriteIterator_type;

                virtual void write(Tp &obj) = 0;

                virtual void write(Tp &&obj) = 0;
            };
        }
    }
}
#endif
