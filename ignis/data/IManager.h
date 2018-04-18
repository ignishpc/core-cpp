
#ifndef IGNIS_IMANAGER_H
#define IGNIS_IMANAGER_H

#include <vector>
#include "serialization/IClassManager.h"

namespace ignis {
    namespace data {
        template<typename T>
        class IManager : public serialization::IClassManager<std::vector<T>> {
        };
    }
}
#endif
