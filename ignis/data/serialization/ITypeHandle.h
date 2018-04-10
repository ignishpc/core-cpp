
#ifndef IGNIS_ITYPEHANDLE_H
#define IGNIS_ITYPEHANDLE_H

#include "IPrinter.h"
#include "IReader.h"
#include "IWriter.h"
#include "IDeleter.h"


namespace ignis {
    namespace data {
        namespace serialization {

            template<typename T>
            class ITypeHandle {
            public:

                IPrinter<T> printer() { return IPrinter<T>(); }

                IReader<T> reader() { return IReader<T>(); }

                IWriter<T> writer() { return IWriter<T>(); }

                IDeleter<T> deleter() { return IDeleter<T>(); }

                const std::type_info &info() { return typeid(T); }
            };
        }
    }
}
#endif
