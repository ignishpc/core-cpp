
#ifndef IGNIS_IWRITER_H
#define IGNIS_IWRITER_H

#include <string>
#include <utility>
#include <list>
#include <forward_list>
#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <memory>
#include <thrift/protocol/TProtocol.h>
#include "IEnumTypes.h"
#include "../../exception/ILogicError.h"
#include "../RTTInfo.h"

namespace ignis {
    namespace data {
        namespace serialization {

            typedef apache::thrift::protocol::TProtocol IProtocol;

            inline void writeSize(IProtocol &protocol, int64_t size) {
                protocol.writeI64(size);
            }

            inline void writeType(IProtocol &protocol, int8_t size) {
                protocol.writeByte(size);
            }

            template<typename T>
            class IWriter {
            public:

                void writeType(IProtocol &protocol) {
                    throw exception::ILogicError(
                            "IWriter not implemented for " + RTTInfo(typeid(T)).getStandardName());
                }

                void operator()(T &obj, IProtocol &protocol) {
                    throw exception::ILogicError(
                            "IWriter not implemented for " + RTTInfo(typeid(T)).getStandardName());
                }
            };

            template<>
            struct IWriter<bool> {
                inline void writeType(IProtocol &protocol) {
                    serialization::writeType(protocol, IEnumTypes::I_BOOL);
                }

                inline void operator()(const bool &obj, IProtocol &protocol) {
                    protocol.writeBool(obj);
                }
            };

            template<>
            struct IWriter<int8_t> {
                inline void writeType(IProtocol &protocol) {
                    serialization::writeType(protocol, IEnumTypes::I_I08);
                }

                inline void operator()(const int8_t &obj, IProtocol &protocol) {
                    protocol.writeByte(obj);
                }
            };

            template<>
            struct IWriter<uint8_t> {
                inline void writeType(IProtocol &protocol) {
                    serialization::writeType(protocol, IEnumTypes::I_I16);
                }

                inline void operator()(const uint8_t &obj, IProtocol &protocol) {
                    protocol.writeI16((int16_t) obj);
                }
            };

            template<>
            struct IWriter<int16_t> {
                inline void writeType(IProtocol &protocol) {
                    serialization::writeType(protocol, IEnumTypes::I_I16);
                }

                inline void operator()(const int16_t &obj, IProtocol &protocol) {
                    protocol.writeI16(obj);
                }
            };

            template<>
            struct IWriter<uint16_t> {
                inline void writeType(IProtocol &protocol) {
                    serialization::writeType(protocol, IEnumTypes::I_I32);
                }

                inline void operator()(const uint16_t &obj, IProtocol &protocol) {
                    protocol.writeI32((int32_t) obj);
                }
            };

            template<>
            struct IWriter<int32_t> {
                inline void writeType(IProtocol &protocol) {
                    serialization::writeType(protocol, IEnumTypes::I_I32);
                }

                inline void operator()(const int32_t &obj, IProtocol &protocol) {
                    protocol.writeI32(obj);
                }
            };

            template<>
            struct IWriter<uint32_t> {
                inline void writeType(IProtocol &protocol) {
                    serialization::writeType(protocol, IEnumTypes::I_I64);
                }

                inline void operator()(const uint32_t &obj, IProtocol &protocol) {
                    protocol.writeI64((int64_t) obj);
                }
            };

            template<>
            struct IWriter<int64_t> {
                inline void writeType(IProtocol &protocol) {
                    serialization::writeType(protocol, IEnumTypes::I_I64);
                }

                inline void operator()(const int64_t &obj, IProtocol &protocol) {
                    protocol.writeI64(obj);
                }
            };

            template<>
            struct IWriter<uint64_t> {
                inline void writeType(IProtocol &protocol) {
                    serialization::writeType(protocol, IEnumTypes::I_PAIR);
                    serialization::writeType(protocol, IEnumTypes::I_I64);
                    serialization::writeType(protocol, IEnumTypes::I_I64);
                }

                inline void operator()(const uint64_t &obj, IProtocol &protocol) {
                    protocol.writeI64((int64_t) (obj >> 32));
                    protocol.writeI64((int64_t) (obj & 0x00000000FFFFFFFF));
                }
            };

            template<>
            struct IWriter<float> {
                inline void writeType(IProtocol &protocol) {
                    serialization::writeType(protocol, IEnumTypes::I_DOUBLE);
                }

                inline void operator()(const float &obj, IProtocol &protocol) {
                    protocol.writeDouble(obj);
                }
            };

            template<>
            struct IWriter<double> {
                inline void writeType(IProtocol &protocol) {
                    serialization::writeType(protocol, IEnumTypes::I_DOUBLE);
                }

                inline void operator()(const double &obj, IProtocol &protocol) {
                    protocol.writeDouble(obj);
                }
            };

            template<>
            struct IWriter<std::string> {
                inline void writeType(IProtocol &protocol) {
                    serialization::writeType(protocol, IEnumTypes::I_STRING);
                }

                inline void operator()(const std::string &obj, IProtocol &protocol) {
                    protocol.writeString(obj);
                }
            };

            template<typename T>
            struct IWriter<std::vector<T>> {
                inline void writeType(IProtocol &protocol) {
                    serialization::writeType(protocol, IEnumTypes::I_LIST);
                }

                inline void operator()(const std::vector<T> &obj, IProtocol &protocol) {
                    writeSize(protocol, obj.size());
                    auto writer = IWriter<T>();
                    writer.writeType(protocol);
                    for (auto it = obj.begin(); it != obj.end(); it++) {
                        writer(*it);
                    }
                }
            };

            template<>
            struct IWriter<std::vector<uint8_t >> {
                inline void writeType(IProtocol &protocol) {
                    serialization::writeType(protocol, IEnumTypes::I_BINARY);
                }

                inline void operator()(const std::vector<uint8_t> &obj, IProtocol &protocol) {
                    auto size = obj.size();
                    writeSize(protocol, size);
                    auto data = obj.data();
                    for (decltype(size) i = 0; i < size; i++) {
                        protocol.writeByte(*(int8_t*)data);
                    }
                }
            };

            template<typename T>
            struct IWriter<std::list<T>> {
                inline void writeType(IProtocol &protocol) {
                    serialization::writeType(protocol, IEnumTypes::I_LIST);
                }

                inline void operator()(const std::list<T> &obj, IProtocol &protocol) {
                    writeSize(protocol, obj.size());
                    auto writer = IWriter<T>();
                    writer.writeType(protocol);
                    for (auto it = obj.begin(); it != obj.end(); it++) {
                        writer(*it);
                    }
                }
            };

            template<typename T>
            struct IWriter<std::forward_list<T>> {
                inline void writeType(IProtocol &protocol) {
                    serialization::writeType(protocol, IEnumTypes::I_LIST);
                }

                inline void operator()(const std::forward_list<T> &obj, IProtocol &protocol) {
                    size_t size = 0;
                    for (auto it = obj.begin(); it != obj.end(); it++) {
                        size++;
                    }
                    writeSize(protocol, size);
                    auto writer = IWriter<T>();
                    writer.writeType(protocol);
                    for (auto it = obj.begin(); it != obj.end(); it++) {
                        writer(*it);
                    }
                }
            };

            template<typename T>
            struct IWriter<std::set<T>> {
                inline void writeType(IProtocol &protocol) {
                    serialization::writeType(protocol, IEnumTypes::I_SET);
                }

                inline void operator()(const std::set<T> &obj, IProtocol &protocol) {
                    writeSize(protocol, obj.size());
                    auto writer = IWriter<T>();
                    writer.writeType(protocol);
                    for (auto it = obj.begin(); it != obj.end(); it++) {
                        writer(*it);
                    }
                }
            };


            template<typename T>
            struct IWriter<std::unordered_set<T>> {
                inline void writeType(IProtocol &protocol) {
                    serialization::writeType(protocol, IEnumTypes::I_SET);
                }

                inline void operator()(const std::unordered_set<T> &obj, IProtocol &protocol) {
                    writeSize(protocol, obj.size());
                    auto writer = IWriter<T>();
                    writer.writeType(protocol);
                    for (auto it = obj.begin(); it != obj.end(); it++) {
                        writer(*it);
                    }
                }
            };

            template<typename K, typename V>
            struct IWriter<std::map<K, V>> {
                inline void writeType(IProtocol &protocol) {
                    serialization::writeType(protocol, IEnumTypes::I_MAP);
                }

                inline void operator()(const std::map<K, V> &obj, IProtocol &protocol) {
                    writeSize(protocol, obj.size());
                    auto w_key = IWriter<K>();
                    auto w_value = IWriter<V>();
                    w_key.writeType(protocol);
                    w_value.writeType(protocol);
                    for (auto it = obj.begin(); it != obj.end(); it++) {
                        w_key(it->first);
                        w_value(it->second);
                    }
                }
            };

            template<typename K, typename V>
            struct IWriter<std::unordered_map<K, V>> {
                inline void writeType(IProtocol &protocol) {
                    serialization::writeType(protocol, IEnumTypes::I_MAP);
                }

                inline void operator()(const std::unordered_map<K, V> &obj, IProtocol &protocol) {
                    writeSize(protocol, obj.size());
                    auto w_key = IWriter<K>();
                    auto w_value = IWriter<V>();
                    w_key.writeType(protocol);
                    w_value.writeType(protocol);
                    for (auto it = obj.begin(); it != obj.end(); it++) {
                        w_key(it->first);
                        w_value(it->second);
                    }
                }
            };

            template<typename T1, typename T2>
            struct IWriter<std::pair<T1, T2>> {
                inline void writeType(IProtocol &protocol) {
                    serialization::writeType(protocol, IEnumTypes::I_PAIR);
                }

                inline void operator()(const std::pair<T1, T2> &obj, IProtocol &protocol) {
                    auto w_first = IWriter<T1>();
                    auto w_second = IWriter<T2>();
                    w_first.writeType(protocol);
                    w_second.writeType(protocol);
                    w_first(obj.first);
                    w_second(obj.second);
                }
            };

            template<typename T>
            struct IWriter<T *> {
                inline void writeType(IProtocol &protocol) {
                    serialization::writeType(protocol, IEnumTypes::I_STRING);
                }

                inline void operator()(const T *&obj, IProtocol &protocol) {
                    protocol.writeString(obj);
                }
            };

            template<typename T>
            struct IWriter<std::shared_ptr<T>> {
                inline void writeType(IProtocol &protocol) {
                    serialization::writeType(protocol, IEnumTypes::I_STRING);
                }

                inline void operator()(const std::shared_ptr<T> &obj, IProtocol &protocol) {
                    protocol.writeString(obj);
                }
            };
        }
    }
}

#endif
