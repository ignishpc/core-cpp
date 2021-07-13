
#ifndef IGNIS_IPROPERTIES_H
#define IGNIS_IPROPERTIES_H

#include "Ignis.h"
#include <map>

namespace ignis {
    namespace driver {
        namespace api {
            class ICluster;

            class IProperties {
            public:
                class Value {
                public:
                    std::string operator=(const std::string &value);

                    std::string value;

                private:
                    friend IProperties;
                    Value(IProperties &properties, const std::string &key, const std::string &value);

                    IProperties &properties;
                    std::string key;
                };

                IProperties();

                IProperties(const IProperties &properties);

                std::string set(const std::string &key, const std::string &value);

                std::string get(const std::string &key);

                std::string rm(const std::string &key);

                Value operator[](const std::string &key);

                bool contains(const std::string &key);

                std::map<std::string, std::string> toMap(bool defaults);

                void fromMap(const std::map<std::string, std::string> &map);

                void load(const std::string &path);

                void store(const std::string &path);

                void clear();

            private:
                friend ICluster;
                int64_t id;
            };
        }// namespace api
    }    // namespace driver
}// namespace ignis

#endif
