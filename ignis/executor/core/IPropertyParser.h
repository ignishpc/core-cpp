
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

                int64_t minPartitionSize() { return getNumber("ignis.partition.minSize"); }

                int64_t sortSamples() { return getNumber("ignis.modules.sort.samples"); }

                int8_t msgCompression() { return getNumber("ignis.transport.compression"); }

                int8_t partitionCompression() { return getNumber("ignis.partition.compression"); }

                bool nativeSerialization() { return getString("ignis.partition.serialization") == "native"; }

                std::string partitionType() { return getString("ignis.partition.type"); }

                std::string workingDirectory() { return getString("ignis.job.directory"); }

                std::string jobName() { return getString("ignis.job.name"); }

                std::string partitionDirectory() { return getString("ignis.partition.directory"); }

                //Auxiliary functions

                std::string &getString(const std::string &key);

                int64_t getNumber(const std::string &key);

                size_t getSize(const std::string &key);

                bool getBoolean(const std::string &key);

            private:
                std::unordered_map<std::string, std::string> &properties;

            };
        }
    }
}

#endif
