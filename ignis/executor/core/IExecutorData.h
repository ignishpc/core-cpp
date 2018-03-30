
#ifndef EXECUTORCPP_EXECUTORDATA_H
#define EXECUTORCPP_EXECUTORDATA_H

#include <memory>
#include <unordered_map>
#include "storage/IObject.h"

namespace ignis {
    namespace executor {
        namespace core {
            class IExecutorData {
            public:
            private:
                std::shared_ptr<storage::IObject> loaded_object;
                std::unordered_map<size_t, std::shared_ptr<storage::IObject>> stored;
            };
        }
    }
}

#endif
