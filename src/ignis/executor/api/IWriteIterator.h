
#ifndef IGNIS_IWRITEITERATOR_H
#define IGNIS_IWRITEITERATOR_H

#include "ignis/executor/core/exception/ILogicError.h"

namespace ignis {
    namespace executor {
        namespace api {
            template<typename Tp>
            class IWriteIterator {
            public:
                typedef Tp value_type;
                typedef IWriteIterator<Tp> _IWriteIterator_type;

                virtual void write(Tp &obj) { throw core::exception::ILogicError("not implemented"); }

                virtual void write(Tp &&obj) { throw core::exception::ILogicError("not implemented"); }
            };
        }// namespace api
    }    // namespace executor
}// namespace ignis
#endif
