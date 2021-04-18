
#ifndef IGNIS_ICORETYPES_H
#define IGNIS_ICORETYPES_H

#include "ignis/executor/core/selector/ISelector.h"
#include <vector>

namespace ignis {
    namespace executor {
        namespace core {
            class ICoreTypes {
            public:
                static std::vector<std::shared_ptr<selector::ITypeSelector>> defaultTypes();

                static std::vector<std::shared_ptr<selector::ISelectorGroup>> defaultFunctions();
            };
        }// namespace core
    }    // namespace executor
}// namespace ignis

#endif
