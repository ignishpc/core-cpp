
#ifndef INGIS_OBJECTLOADER_H
#define INGIS_OBJECTLOADER_H

#include "../../IHeaders.h"

namespace ignis {
    namespace executor {
        namespace core {
            class IObjectLoader {
            public:

                template<typename T>
                static std::shared_ptr<T> load(const std::string &name) {
                    return std::static_pointer_cast<T>(innerload(name));
                }

            private:
                static std::shared_ptr<void> innerload(const std::string &name);

            };

        }
    }
}
#endif
