
#ifndef IGNIS_IGNISCONTEXT_H
#define IGNIS_IGNISCONTEXT_H

#include <memory>
#include <string>
#include <unordered_map>

namespace ignis {
    namespace executor {
        namespace api {
            class IContext {
            public:

                IContext(int64_t &id);

                std::string &operator[](const std::string &key);

                std::string &operator[](const std::string key);

                std::string &operator[](const char * key);

                std::unordered_map<std::string, std::string> &getProperties();

                int64_t getId();

            private:
                int64_t& id;
                std::unordered_map<std::string, std::string> properties;
            };
        }
    }
}

#endif
