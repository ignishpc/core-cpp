
#ifndef IGNIS_IWRITEITERATOR_H
#define IGNIS_IWRITEITERATOR_H

#include "executor/core/exception/ILogicError.h"

namespace ignis {
    namespace executor {
        namespace api {
            template<typename Tp>
            class IWriteIterator {
            public:
                typedef Tp value_type;
                typedef IReadIterator <Tp> _IWriteIterator_type;

                virtual void write(Tp &obj) { throw core::exception::ILogicError("not implemented"); }

                virtual void write(Tp &&obj) { throw core::exception::ILogicError("not implemented"); }
            };
        }
    }
}
#endif
