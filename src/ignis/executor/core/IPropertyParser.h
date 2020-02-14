
#ifndef IGNIS_IPROPERTYPARSER_H
#define IGNIS_IPROPERTYPARSER_H

#include <unordered_map>

namespace ignis {
    namespace executor {
        namespace core {
            class IPropertyParser {
            public:
                IPropertyParser(std::unordered_map<std::string, std::string> &properties);

                virtual ~IPropertyParser();

                //All properties

                int cores() { return getNumber("ignis.executor.cores"); }

                int64_t partitionMinimal() { return getSize("ignis.partition.minimal"); }

                int64_t sortSamples() { return getNumber("ignis.modules.sort.samples"); }

                int64_t ioOverwrite() { return getBoolean("ignis.modules.io.overwrite"); }

                int8_t ioCompression() { return getNumber("ignis.modules.io.compression"); }

                int8_t msgCompression() { return getNumber("ignis.transport.compression"); }

                int8_t partitionCompression() { return getNumber("ignis.partition.compression"); }

                bool nativeSerialization() { return getString("ignis.partition.serialization") == "native"; }

                std::string partitionType() { return getString("ignis.partition.type"); }

                std::string jobName() { return getString("ignis.job.name"); }

                std::string JobDirectory() { return getString("ignis.job.directory"); }

                //Auxiliary functions

                std::string &getString(const std::string &key);

                int64_t getNumber(const std::string &key);

                size_t getSize(const std::string &key);

                bool getBoolean(const std::string &key);

            private:
                void parserError(std::string key, std::string value, size_t pos);

                std::unordered_map<std::string, std::string> &properties;

            };
        }
    }
}

#endif
