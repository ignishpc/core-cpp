
#ifndef IGNIS_IPROPERTIESPARSER_H
#define IGNIS_IPROPERTIESPARSER_H

#include <string>
#include <unordered_map>

namespace ignis {
    namespace executor {
        namespace core {
            class IPropertiesParser {
            public:
                IPropertiesParser(std::unordered_map<std::string, std::string> &properties);

                size_t getNumber(std::string key);

                size_t getSize(std::string key);

                bool getBoolean(std::string key);

                virtual ~IPropertiesParser();

            private:
                std::unordered_map<std::string, std::string> &properties;


            };
        }
    }
}

#endif
