
#ifndef IGNIS_IJSONVALUE_H
#define IGNIS_IJSONVALUE_H

#include "ignis/executor/core/io/IWriter.h"
#include "ignis/executor/core/io/IReader.h"
#include "ignis/executor/core/io/IJsonWriter.h"
#include "ignis/executor/core/io/IJsonReader.h"
#include "ignis/executor/core/io/IPrinter.h"

namespace ignis {
    namespace executor {
        namespace api {
            class IJsonValue {
            public:

                IJsonValue();

                IJsonValue(bool value);

                IJsonValue(int64_t value);

                IJsonValue(double value);

                IJsonValue(const std::string &value);

                IJsonValue(std::string &&value);

                IJsonValue(const std::vector<IJsonValue> &value);

                IJsonValue(std::vector<IJsonValue> &&value);

                IJsonValue(const std::unordered_map<std::string, IJsonValue> &value);

                IJsonValue(std::unordered_map<std::string, IJsonValue> &&value);

                bool isNull() const;

                void setNull();

                bool isBoolean() const;

                bool getBoolean() const;

                void setBoolean(bool value);

                bool isInteger() const;

                int64_t getInteger() const;

                void setInteger(int64_t value);

                bool isDouble() const;

                double getDouble() const;

                void setDouble(double value);

                bool isNumber() const;

                double getNumber() const;

                bool isString() const;

                const std::string &getString() const;

                void setString(const std::string &value);

                void setString(std::string &&value);

                bool isArray() const;

                const std::vector<IJsonValue> &getArray() const;

                void setArray(const std::vector<IJsonValue> &value);

                void setArray(std::vector<IJsonValue> &&value);

                bool isMap() const;

                const std::unordered_map<std::string, IJsonValue> &getMap() const;

                void setMap(const std::unordered_map<std::string, IJsonValue> &value);

                void setMap(std::unordered_map<std::string, IJsonValue> &&value);

                int getTypeId() const;

            private:
                inline void reset();

                union {
                    bool boolean;
                    int64_t integer;
                    double floating;
                    void *pointer;
                } elem;
                int8_t type;
            };
        }
        namespace core {
            namespace io {

                template<>
                struct IJsonWriterType<api::IJsonValue> {
                    inline void operator()(JsonWriter &out, const api::IJsonValue &v) {
                        writers[v.getTypeId()](out, v);
                    }

                private:

                    static void (*writers[])(JsonWriter &out, const api::IJsonValue &v);
                };

                template<>
                struct IJsonReaderType<api::IJsonValue> {
                    inline void operator()(JsonNode &in, api::IJsonValue &v) {
                        readers[in.GetType()](in, v);
                    }

                    inline api::IJsonValue operator()(JsonNode &in) {
                        api::IJsonValue obj;
                        (*this)(in, obj);
                        return obj;
                    }

                private:

                    static void (*readers[])(JsonNode &in, api::IJsonValue &v);
                };

                template<>
                struct IWriterType<api::IJsonValue> {

                    inline void writeType(protocol::IProtocol &protocol) {
                        writeTypeAux(protocol, IEnumTypes::I_JSON);
                    }

                    inline void operator()(protocol::IProtocol &protocol, const api::IJsonValue &v) {
                        writers[v.getTypeId()](protocol, v);
                    }

                private:

                    static void (*writers[])(protocol::IProtocol &protocol, const api::IJsonValue &obj);
                };

                template<>
                struct IReaderType<api::IJsonValue> {

                    inline bool readType(protocol::IProtocol &protocol) {
                        return IEnumTypes::I_JSON == readTypeAux(protocol);
                    }

                    inline void operator()(protocol::IProtocol &protocol, api::IJsonValue &v) {
                        readers[readTypeAux(protocol)](protocol, v);
                    }

                    inline api::IJsonValue operator()(protocol::IProtocol &protocol) {
                        api::IJsonValue obj;
                        (*this)(protocol, obj);
                        return std::move(obj);
                    }

                private:

                    static void (*readers[])(protocol::IProtocol &protocol, api::IJsonValue &obj);
                };

                template<>
                struct IPrinterType<api::IJsonValue> {

                    inline void operator()(std::ostream &out, const api::IJsonValue &v, int64_t level) {
                        printers[v.getTypeId()](out, v, level);
                    }

                private:
                    static void (*printers[])(std::ostream &out, const api::IJsonValue &v, int64_t level);
                };
            }
        }
    }
}

#endif
