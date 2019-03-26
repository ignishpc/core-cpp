
#ifndef IGNIS_IGNISCONTEXT_H
#define IGNIS_IGNISCONTEXT_H

#include "../../IHeaders.h"
#include "IManager.h"
#include <unordered_map>

namespace ignis {
    namespace executor {
        namespace api {
            class IContext {
            public:

                IContext(void *loader);

                std::string &operator[](const std::string &key);

                std::unordered_map<std::string, std::string> &getProperties();

                template<typename T>
                std::shared_ptr<T> getVariable(const std::string &name) {
                    if (containsVariable(name)) {
                        auto &var = variables[name];
                        if (!var.first) {
                            var.second = decodeVariable(var.second,
                                                        std::static_pointer_cast<void>(
                                                                std::make_shared<IManager<T>>()));
                            var.first = true;
                        }
                        return std::static_pointer_cast<T>(var.second);
                    }
                }

                void removeVariable(const std::string &name);

                bool containsVariable(const std::string &name);

                std::unordered_map<std::string, std::pair<bool, std::shared_ptr<void>>> &getVariables();

                void removeVariables();



                template<typename T>
                void registerManager() {
                    vregisterManager(std::static_pointer_cast<void>(std::make_shared<IManager<T>>()));
                }

                virtual ~IContext();

            private:
                std::unordered_map<std::string, std::string> properties;
                std::unordered_map<std::string, std::pair<bool, std::shared_ptr<void>>> variables;

                std::shared_ptr<void> decodeVariable(std::shared_ptr<void> var, std::shared_ptr<void> manager);

                void vregisterManager(std::shared_ptr<void> manager);

                void *loader;
            };
        }
    }
}

#endif
