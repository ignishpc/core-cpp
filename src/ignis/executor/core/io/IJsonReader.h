
#ifndef IGNIS_IJSONREADER_H
#define IGNIS_IJSONREADER_H

#include <istream>
#include "ignis/executor/core/RTTInfo.h"
#include "ignis/executor/core/exception/ILogicError.h"
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/document.h>


namespace ignis {
    namespace executor {
        namespace core {
            namespace io {

                typedef rapidjson::Document::ValueType JsonNode;

                template<typename T>
                inline void checkJsonTypeAux(bool valid) {
                    if (!valid) {
                        throw exception::ILogicError(
                                "IJsonReaderType type error for " + RTTInfo::from<T>().getStandardName());
                    }
                }

                template<typename T>
                struct IJsonReaderType {
                    inline void operator()(JsonNode &in, T &obj) {
                        throw exception::ILogicError(
                                "IJsonReaderType not implemented for " + RTTInfo(typeid(T)).getStandardName());
                    }

                    inline T operator()(JsonNode &in) {
                        throw exception::ILogicError(
                                "IJsonReaderType not implemented for " + RTTInfo(typeid(T)).getStandardName());
                    }
                };

                template<typename T>
                class IJsonReader {
                public:

                    virtual inline T operator()(std::istream &in) {
                        rapidjson::IStreamWrapper wrapper(in);
                        rapidjson::Document root;
                        root.ParseStream<rapidjson::kParseIterativeFlag>(wrapper);
                        return reader(root);
                    }

                    virtual inline void operator()(std::istream &in, T &obj) {
                        rapidjson::IStreamWrapper wrapper(in);
                        rapidjson::Document root;
                        root.ParseStream<rapidjson::kParseIterativeFlag>(wrapper);
                        reader(root, obj);
                    }

                private:
                    IJsonReaderType<T> reader;

                };

            }
        }
    }
}


#include "IJsonReader.tcc"

#endif
