
#ifndef INGIS_OBJECTLOADER_H
#define INGIS_OBJECTLOADER_H

#include "../../IHeaders.h"
#include "../api/IManager.h"
#include "storage/IObject.h"
#include <unordered_map>

namespace ignis {
    namespace executor {
        namespace core {
            class IObjectLoader {
            public:

                template<typename T>
                std::shared_ptr<T> load(const std::string &name) {
                    return std::static_pointer_cast<T>(vload(name));
                }

                std::shared_ptr<storage::IObject::Any> loadVariable(const std::string &bytes,
                                                                    std::shared_ptr<executor::api::IManager<storage::IObject::Any>> manager);

                std::shared_ptr<executor::api::IManager<storage::IObject::Any>> getManager(const std::string &id);

                std::shared_ptr<executor::api::IManager<storage::IObject::Any>> compileManager(const std::string &id);

                void registermanager(std::shared_ptr<executor::api::IManager<storage::IObject::Any>> manager);

                bool containsManager(const std::string &id);

                std::string parseId(storage::IObject &obj);

            private:
                std::string parseId(executor::api::IManager<storage::IObject::Any> &manager);

                std::shared_ptr<void> vload(const std::string &name);

                std::unordered_map<std::string, std::shared_ptr<void>> libraries;
            };

        }
    }
}
#endif
