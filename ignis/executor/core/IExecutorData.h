
#ifndef IGNIS_EXECUTORDATA_H
#define IGNIS_EXECUTORDATA_H

#include "../../IHeaders.h"
#include "IObjectLoader.h"
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

                std::shared_ptr<storage::IObject> loadObject(std::shared_ptr<storage::IObject> object);

                std::shared_ptr<storage::IObject> loadObject();

                void deleteLoadObject();

                api::IContext &getContext();

                core::IPropertiesParser &getParser();

                IPostBox &getPostBox();

                int64_t getThreads();

                IObjectLoader &getObjectLoader();

                virtual ~IExecutorData();

            private:
                std::shared_ptr<storage::IObject> loaded_object;
                IObjectLoader object_loader;
                IPostBox post_box;
                api::IContext context;
                core::IPropertiesParser properties_parser;
            };
        }
    }
}

#endif
