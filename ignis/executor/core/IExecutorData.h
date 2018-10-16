
#ifndef IGNIS_EXECUTORDATA_H
#define IGNIS_EXECUTORDATA_H

#include <unordered_map>
#include "../../IHeaders.h"
#include "../api/IContext.h"
#include "IPropertiesParser.h"
#include "storage/IObject.h"
#include "IPostBox.h"

namespace ignis {
    namespace executor {
        namespace core {
            class IExecutorData {
            public:

                IExecutorData();

                void loadObject(std::shared_ptr<storage::IObject> object);

                bool hasLoadObject();

                void deleteLoadObject();

                storage::IObject &getLoadObject();

                std::shared_ptr<storage::IObject> getSharedLoadObject();

                api::IContext &getContext();

                core::IPropertiesParser &getParser();

                IPostBox &getPostBox();

                int64_t getThreads();

                virtual ~IExecutorData();

            private:
                std::shared_ptr<storage::IObject> loaded_object;
                IPostBox post_box;
                api::IContext context;
                core::IPropertiesParser properties_parser;
            };
        }
    }
}

#endif
