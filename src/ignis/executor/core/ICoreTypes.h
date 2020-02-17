
#ifndef IGNIS_ICORETYPES_H
#define IGNIS_ICORETYPES_H

#include <vector>
#include "ignis/executor/core/selector/ITypeSelector.h"

namespace ignis{
    namespace executor{
        namespace core{
            class ICoreTypes {
            public:
                static std::vector<std::shared_ptr<selector::ITypeSelector>> defaultTypes();

            };
        }
    }
}

#endif
