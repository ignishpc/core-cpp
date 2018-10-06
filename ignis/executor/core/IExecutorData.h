
#ifndef IGNIS_EXECUTORDATA_H
#define IGNIS_EXECUTORDATA_H

#include "../../IHeaders.h"
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

                void loadObject(std::shared_ptr<storage::IObject> object);

                bool hasLoadObject();

                void deleteLoadObject();

                storage::IObject &getLoadObject();

                std::shared_ptr<storage::IObject> getSharedLoadObject();

                api::IContext &getContext();

                core::IPropertiesParser &getParser();

                storage::IPostBox &getPostBox();

                int64_t& getExecutorId();

                int64_t getThreads();

                virtual ~IExecutorData();

            private:
                int64_t executor_id;
                int64_t threads;
                std::shared_ptr<storage::IObject> loaded_object;
                storage::IPostBox post_box;
                api::IContext context;
                core::IPropertiesParser properties_parser;
            };
        }
    }
}

#endif
