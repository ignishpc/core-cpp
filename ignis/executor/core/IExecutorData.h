
#ifndef IGNIS_EXECUTORDATA_H
#define IGNIS_EXECUTORDATA_H

#include <memory>
#include <unordered_map>
#include "../api/IContext.h"
#include "IPropertiesParser.h"
#include "storage/IObject.h"
#include "storage/IPostBox.h"

namespace ignis {
    namespace executor {
        namespace core {
            class IExecutorData {
            public:

                IExecutorData();

                void loadObject(std::shared_ptr<storage::IObject> &object);

                bool hasLoadObject();

                void deleteLoadObject();

                storage::IObject &getLoadObject();

                std::unordered_map<size_t, std::shared_ptr<storage::IObject>>& getStoredObjects();

                api::IContext &getContext();

                core::IPropertiesParser &getParser();

                storage::IPostBox &getPostBox();

                int64_t& getExecutorId();

                virtual ~IExecutorData();

            private:
                int64_t executor_id;
                std::shared_ptr<storage::IObject> loaded_object;
                std::unordered_map<size_t, std::shared_ptr<storage::IObject>> stored_objects;
                storage::IPostBox post_box;
                api::IContext context;
                core::IPropertiesParser properties_parser;
            };
        }
    }
}

#endif
