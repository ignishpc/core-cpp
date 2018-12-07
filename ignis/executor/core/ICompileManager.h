

#ifndef IGNIS_ICOMPILEMANAGER_H
#define IGNIS_ICOMPILEMANAGER_H

#include "storage/IObject.h"
#include <thrift/transport/TTransport.h>

namespace ignis {
    namespace executor {
        namespace core {
            class ICompileManager {
            public:
                ICompileManager(storage::IObject &obj);

                virtual std::string compile();

                virtual ~ICompileManager();
            private:
                std::string parseType();

                storage::IObject &obj;

            };
        }
    }
}
#endif
