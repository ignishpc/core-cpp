
#ifndef IGNIS_IPROPERTYPARSER_H
#define IGNIS_IPROPERTYPARSER_H

#include "exception/IInvalidArgument.h"
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

                double transportCores() { return getRangeDouble("ignis.transport.cores", 0, 1); }

                int64_t partitionMinimal() { return getSize("ignis.partition.minimal"); }

                double sortSamples() { return getMinDouble("ignis.modules.sort.samples", 0); }

                bool sortResampling() { return getBoolean("ignis.modules.sort.resampling"); }

                int64_t ioOverwrite() { return getBoolean("ignis.modules.io.overwrite"); }

                double ioCores() { return getRangeDouble("ignis.modules.io.cores", 0, 1); }

                int8_t ioCompression() { return getRangeNumber("ignis.modules.io.compression", 0, 9); }

                int8_t msgCompression() { return getRangeNumber("ignis.transport.compression", 0, 9); }

                int8_t partitionCompression() { return getRangeNumber("ignis.partition.compression", 0, 9); }

                int64_t transportElemSize() { return getSize("ignis.transport.element.size"); }

                std::string partitionType() { return getString("ignis.partition.type"); }

                std::string jobDirectory() { return getString("ignis.job.directory"); }

                std::string executorDirectory() { return getString("ignis.executor.directory"); }

                //Auxiliary functions

                std::string &getString(const std::string &key);

                int64_t getNumber(const std::string &key);

                double getDouble(const std::string &key);

                int64_t getMinNumber(const std::string &key, const int64_t min);

                int64_t getMaxNumber(const std::string &key, const int64_t max);

                int64_t getRangeNumber(const std::string &key, const int64_t min, const int64_t max);

                double getMinDouble(const std::string &key, const double min);

                double getMaxDouble(const std::string &key, const double max);

                double getRangeDouble(const std::string &key, const double min, const double max);

                size_t getSize(const std::string &key);

                bool getBoolean(const std::string &key);

            private:
                void parserError(const std::string &key, const std::string &value, size_t pos);

                std::unordered_map<std::string, std::string> &properties;
            };
        }// namespace core
    }    // namespace executor
}// namespace ignis

#endif
