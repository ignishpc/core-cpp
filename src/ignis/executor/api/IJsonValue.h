
#ifndef IGNIS_IJSONVALUE_H
#define IGNIS_IJSONVALUE_H

#include "ignis/executor/core/io/IWriter.h"
#include "ignis/executor/core/io/IReader.h"
#include "ignis/executor/core/io/IJsonWriter.h"
#include "ignis/executor/core/io/IJsonReader.h"

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

                int64_t getDouble() const;

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

            private:
                std::shared_ptr<void> elem;
                int type;
            };
        }
        namespace core {
            namespace io {

                template<>
                struct IJsonWriterType<api::IJsonValue> {
                    inline void operator()(JsonWriter &out, const api::IJsonValue &v) {
                        if (writer == nullptr) {
                            if (v.isBoolean()) {
                                writer = [](JsonWriter &out, const api::IJsonValue &v) {
                                    IJsonWriterType<bool>()(out, v.getBoolean());
                                };
                            } else if (v.isInteger()) {
                                writer = [](JsonWriter &out, const api::IJsonValue &v) {
                                    IJsonWriterType<int64_t>()(out, v.getInteger());
                                };
                            } else if (v.isDouble()) {
                                writer = [](JsonWriter &out, const api::IJsonValue &v) {
                                    IJsonWriterType<double>()(out, v.getDouble());
                                };
                            } else if (v.isString()) {
                                writer = [](JsonWriter &out, const api::IJsonValue &v) {
                                    IJsonWriterType<std::string>()(out, v.getString());
                                };
                            } else if (v.isArray()) {
                                writer = [](JsonWriter &out, const api::IJsonValue &v) {
                                    IJsonWriterType<std::vector<api::IJsonValue>>()(out, v.getArray());
                                };
                            } else if (v.isMap()) {
                                writer = [](JsonWriter &out, const api::IJsonValue &v) {
                                    IJsonWriterType<std::unordered_map<std::string, api::IJsonValue>>()(
                                            out, v.getMap()
                                    );
                                };
                            } else {
                                writer = [](JsonWriter &out, const api::IJsonValue &v) {
                                    out.Null();
                                };//Null
                            }
                        }
                        writer(out, v);
                    }

                private:
                    void (*writer)(JsonWriter &out, const api::IJsonValue &v) = nullptr;
                };

                template<>
                struct IJsonReaderType<api::IJsonValue> {
                    inline void operator()(JsonNode &in, api::IJsonValue &v) {
                        if (reader == nullptr) {
                            if (v.isBoolean()) {
                                reader = [](JsonNode &in, api::IJsonValue &v) {
                                    v.setBoolean(IJsonReaderType<bool>()(in));;
                                };
                            } else if (v.isInteger()) {
                                reader = [](JsonNode &in, api::IJsonValue &v) {
                                    v.setInteger(IJsonReaderType<int64_t>()(in));
                                };
                            } else if (v.isDouble()) {
                                reader = [](JsonNode &in, api::IJsonValue &v) {
                                    v.setDouble(IJsonReaderType<double>()(in));
                                };
                            } else if (v.isString()) {
                                reader = [](JsonNode &in, api::IJsonValue &v) {
                                    v.setString(IJsonReaderType<std::string>()(in));
                                };
                            } else if (v.isArray()) {
                                reader = [](JsonNode &in, api::IJsonValue &v) {
                                    v.setArray(IJsonReaderType<std::vector<api::IJsonValue>>()(in));
                                };
                            } else if (v.isMap()) {
                                reader = [](JsonNode &in, api::IJsonValue &v) {
                                    v.setMap(IJsonReaderType<std::unordered_map<std::string, api::IJsonValue>>()(in));
                                };
                            } else {
                                reader = [](JsonNode &in, api::IJsonValue &v) {
                                    v.setNull();
                                };//Null
                            }
                        }
                        reader(in, v);
                    }


                    inline api::IJsonValue operator()(JsonNode &in) {
                        api::IJsonValue obj;
                        (*this)(in, obj);
                        return obj;
                    }

                private:
                    void (*reader)(JsonNode &in, api::IJsonValue &v) = nullptr;
                };

                template<>
                struct IWriterType<api::IJsonValue> {

                    inline void writeType(protocol::IProtocol &protocol) {
                        writeTypeAux(protocol, IEnumTypes::I_JSON);
                    }

                    inline void operator()(protocol::IProtocol &protocol, const api::IJsonValue &v) {
                        if (writer == nullptr) {
                            if (v.isBoolean()) {
                                writer = [](protocol::IProtocol &protocol, const api::IJsonValue &obj) {
                                    writeTypeAux(protocol, IEnumTypes::I_BOOL);
                                    IWriterType<bool>()(protocol, obj.getBoolean());
                                };
                            } else if (v.isInteger()) {
                                writer = [](protocol::IProtocol &protocol, const api::IJsonValue &obj) {
                                    writeTypeAux(protocol, IEnumTypes::I_I64);
                                    IWriterType<int64_t>()(protocol, obj.getInteger());
                                };
                            } else if (v.isDouble()) {
                                writer = [](protocol::IProtocol &protocol, const api::IJsonValue &obj) {
                                    writeTypeAux(protocol, IEnumTypes::I_DOUBLE);
                                    IWriterType<double>()(protocol, obj.getDouble());
                                };
                            } else if (v.isString()) {
                                writer = [](protocol::IProtocol &protocol, const api::IJsonValue &obj) {
                                    writeTypeAux(protocol, IEnumTypes::I_STRING);
                                    IWriterType<std::string>()(protocol, obj.getString());
                                };
                            } else if (v.isArray()) {
                                writer = [](protocol::IProtocol &protocol, const api::IJsonValue &obj) {
                                    writeTypeAux(protocol, IEnumTypes::I_LIST);
                                    IWriterType<std::vector<api::IJsonValue>>()(protocol, obj.getArray());
                                };
                            } else if (v.isMap()) {
                                writer = [](protocol::IProtocol &protocol, const api::IJsonValue &obj) {
                                    writeTypeAux(protocol, IEnumTypes::I_MAP);
                                    IWriterType<std::unordered_map<std::string, api::IJsonValue>>()(
                                            protocol, obj.getMap()
                                    );
                                };
                            } else {
                                writer = [](protocol::IProtocol &protocol, const api::IJsonValue &obj) {
                                    writeTypeAux(protocol, IEnumTypes::I_VOID);
                                };//Null
                            }
                        }
                        writer(protocol, v);
                    }
                private:
                    void (*writer)(protocol::IProtocol &protocol, const api::IJsonValue &obj) = nullptr;
                };

                template<>
                struct IReaderType<api::IJsonValue> {

                    inline bool readType(protocol::IProtocol &protocol) {
                        return IEnumTypes::I_JSON == readTypeAux(protocol);
                    }

                    inline void operator()(protocol::IProtocol &protocol, api::IJsonValue& v){
                        auto tp = readTypeAux(protocol);
                        if (reader == nullptr) {
                            if (tp == IEnumTypes::I_BOOL) {
                                reader = [](protocol::IProtocol &protocol, api::IJsonValue& obj) {
                                    obj.setBoolean(IReaderType<bool>()(protocol));;
                                };
                            } else if (tp == IEnumTypes::I_I64) {
                                reader = [](protocol::IProtocol &protocol, api::IJsonValue& obj) {
                                    obj.setInteger(IReaderType<int64_t>()(protocol));
                                };
                            } else if (tp == IEnumTypes::I_DOUBLE) {
                                reader = [](protocol::IProtocol &protocol, api::IJsonValue& obj) {
                                    obj.setDouble(IReaderType<double>()(protocol));
                                };
                            } else if (tp == IEnumTypes::I_STRING) {
                                reader = [](protocol::IProtocol &protocol, api::IJsonValue& obj) {
                                    obj.setString(IReaderType<std::string>()(protocol));
                                };
                            } else if (tp == IEnumTypes::I_LIST) {
                                reader = [](protocol::IProtocol &protocol, api::IJsonValue& obj) {
                                    obj.setArray(IReaderType<std::vector<api::IJsonValue>>()(protocol));
                                };
                            } else if (tp == IEnumTypes::I_MAP) {
                                reader = [](protocol::IProtocol &protocol, api::IJsonValue& obj) {
                                    obj.setMap(IReaderType<std::unordered_map<std::string, api::IJsonValue>>()(protocol));
                                };
                            } else {
                                reader = [](protocol::IProtocol &protocol, api::IJsonValue& obj) {
                                    obj.setNull();
                                };//Null
                            }
                        }
                        reader(protocol, v);
                    }

                    inline api::IJsonValue operator()(protocol::IProtocol &protocol) {
                        api::IJsonValue obj;
                        (*this)(protocol, obj);
                        return std::move(obj);
                    }

                private:
                    void (*reader)(protocol::IProtocol &protocol, api::IJsonValue& obj) = nullptr;
                };
            }
        }
    }
}

#endif
