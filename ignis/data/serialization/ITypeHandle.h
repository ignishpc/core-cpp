
#ifndef IGNIS_ITYPEHANDLE_H
#define IGNIS_ITYPEHANDLE_H

#include <memory>
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

                std::shared_ptr<ITypeHandle<T>> handle() { return std::make_shared<ITypeHandle<T>>(); }

                std::shared_ptr<IPrinter<T>> printer() { return std::make_shared<IPrinter<T>>(); }

                std::shared_ptr<IReader<T>> reader() { return std::make_shared<IReader<T>>(); }

                std::shared_ptr<IWriter<T>> writer() { return std::make_shared<IWriter<T>>(); }

                std::shared_ptr<IDeleter<T>> deleter() { return std::make_shared<IDeleter<T>>(); }

                const std::type_info &info() { return typeid(T); }
            };
        }
    }
}
#endif
