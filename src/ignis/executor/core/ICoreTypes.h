
#ifndef IGNIS_ICORETYPES_H
#define IGNIS_ICORETYPES_H

#include "ignis/executor/core/selector/ITypeSelector.h"
#include <vector>

namespace ignis {
    namespace executor {
        namespace core {
            class ICoreTypes {
            public:
                static std::vector<std::shared_ptr<selector::ITypeSelector>> defaultTypes();
            };
        }// namespace core
    }    // namespace executor
}// namespace ignis

#endif
