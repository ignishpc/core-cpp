
#ifndef IGNIS_IJSONWRITER_H
#define IGNIS_IJSONWRITER_H

#include "ignis/executor/core/RTTInfo.h"
#include "ignis/executor/core/exception/ILogicError.h"
#include <ostream>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>

namespace ignis {
    namespace executor {
        namespace core {
            namespace io {

                typedef rapidjson::Writer<rapidjson::OStreamWrapper> JsonWriter;

                template<typename T>
                struct IJsonWriterType {
                    inline void operator()(JsonWriter &out, const T &b) {
                        throw exception::ILogicError("IJsonWriterType not implemented for " +
                                                     RTTInfo::from<T>().getStandardName());
                    }
                };

                template<typename T>
                class IJsonWriter {
                public:
                    virtual void operator()(std::ostream &out, const T &b, bool pretty = true) {
                        rapidjson::OStreamWrapper wrapper(out);
                        if (pretty) {
                            rapidjson::PrettyWriter<rapidjson::OStreamWrapper> json_writer(wrapper);
                            writer((JsonWriter &) json_writer, b);

                        } else {
                            JsonWriter json_writer(wrapper);
                            writer(json_writer, b);
                        }
                    }

                private:
                    IJsonWriterType<T> writer;
                };
            }// namespace io
        }    // namespace core
    }        // namespace executor
}// namespace ignis

#include "IJsonWriter.tcc"

#endif
