
#ifndef IGNIS_IREADER_H
#define IGNIS_IREADER_H

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

            inline int8_t readType(IProtocol &protocol) {
                int8_t type;
                protocol.readByte(type);
                return type;
            }

            inline int64_t readSize(IProtocol &protocol) {
                int64_t size;
                protocol.readI64(size);
                return size;
            }

            template<typename T>
            struct IReader {
                inline void checkType(IProtocol &protocol) {
                    if (readType(protocol)) {
                        throw exception::ILogicError(
                                "IReader type error for" + RTTInfo(typeid(T)).getStandardName());
                    }
                }

                bool readType(IProtocol &protocol) {
                    throw exception::ILogicError(
                            "IReader not implemented for " + RTTInfo(typeid(T)).getStandardName());
                }

                T operator()(IProtocol &protocol) {
                    throw exception::ILogicError(
                            "IReader not implemented for " + RTTInfo(typeid(T)).getStandardName());
                }
            };

            template<>
            struct IReader<bool> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_BOOL == readType(protocol);
                }

                inline bool operator()(IProtocol &protocol) {
                    bool obj;
                    protocol.readBool(obj);
                    return obj;
                }
            };

            template<>
            struct IReader<int8_t> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_I08 == readType(protocol);
                }

                inline int8_t operator()(IProtocol &protocol) {
                    int8_t obj;
                    protocol.readByte(obj);
                    return obj;
                }
            };

            template<>
            struct IReader<uint8_t> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_I16 == readType(protocol);
                }

                inline uint8_t operator()(IProtocol &protocol) {
                    int16_t obj;
                    protocol.readI16(obj);
                    return obj;
                }
            };

            template<>
            struct IReader<int16_t> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_I16 == readType(protocol);
                }

                inline int16_t operator()(IProtocol &protocol) {
                    int16_t obj;
                    protocol.readI16(obj);
                    return obj;
                }
            };

            template<>
            struct IReader<uint16_t> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_I32 == readType(protocol);
                }

                inline uint16_t operator()(IProtocol &protocol) {
                    int32_t obj;
                    protocol.readI32(obj);
                    return obj;
                }
            };

            template<>
            struct IReader<int32_t> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_I32 == readType(protocol);
                }

                inline int32_t operator()(IProtocol &protocol) {
                    int32_t obj;
                    protocol.readI32(obj);
                    return obj;
                }
            };

            template<>
            struct IReader<uint32_t> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_I64 == readType(protocol);
                }

                inline uint32_t operator()(IProtocol &protocol) {
                    int64_t obj;
                    protocol.readI64(obj);
                    return obj;
                }
            };

            template<>
            struct IReader<int64_t> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_I64 == readType(protocol);
                }

                inline int64_t operator()(IProtocol &protocol) {
                    int64_t obj;
                    protocol.readI64(obj);
                    return obj;
                }
            };

            template<>
            struct IReader<uint64_t> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_PAIR == readType(protocol) &&
                           IEnumTypes::I_I64 == readType(protocol) &&
                           IEnumTypes::I_I64 == readType(protocol);
                }

                inline uint64_t operator()(IProtocol &protocol) {
                    uint64_t obj;
                    protocol.readI64((int64_t &) obj);
                    int64_t aux;
                    protocol.readI64(aux);
                    return (obj << 32) & aux;
                }
            };

            template<>
            struct IReader<float> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_DOUBLE == readType(protocol);
                }

                inline float operator()(IProtocol &protocol) {
                    double obj;
                    protocol.readDouble(obj);
                    return (float) obj;
                }
            };

            template<>
            struct IReader<double> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_DOUBLE == readType(protocol);
                }

                inline double operator()(IProtocol &protocol) {
                    double obj;
                    protocol.readDouble(obj);
                    return obj;
                }
            };

            template<>
            struct IReader<std::string> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_STRING == readType(protocol);
                }

                inline std::string operator()(IProtocol &protocol) {
                    std::string obj;
                    protocol.readString(obj);
                    return obj;
                }
            };

            template<typename T>
            struct IReader<std::vector<T>> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_LIST == readType(protocol);
                }

                inline std::vector<T> operator()(IProtocol &protocol) {
                    std::vector<T> obj;
                    auto size = readSize(protocol);
                    auto reader = IReader<T>();
                    reader.checkType(protocol);
                    obj.reserve(size);
                    for (decltype(size) i = 0; i < size; i++) {
                        obj.push_back(std::move(reader(protocol)));
                    }
                    return obj;
                }
            };

            template<>
            struct IReader<std::vector<uint8_t >> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_BINARY == readType(protocol);
                }

                inline std::vector<uint8_t> operator()(IProtocol &protocol) {
                    std::vector<uint8_t> obj;
                    auto size = readSize(protocol);
                    obj.resize(size);
                    auto data = obj.data();
                    for (decltype(size) i = 0; i < size; i++) {
                        protocol.readByte(*(int8_t *) data);
                    }
                    return obj;
                }
            };

            template<typename T>
            struct IReader<std::list<T>> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_LIST == readType(protocol);
                }

                inline std::list<T> operator()(IProtocol &protocol) {
                    std::list<T> obj;
                    auto size = readSize(protocol);
                    auto reader = IReader<T>();
                    reader.checkType(protocol);
                    for (decltype(size) i = 0; i < size; i++) {
                        obj.push_back(std::move(reader(protocol)));
                    }
                    return obj;
                }
            };

            template<typename T>
            struct IReader<std::forward_list<T>> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_LIST == readType(protocol);
                }

                inline std::forward_list<T> operator()(IProtocol &protocol) {
                    std::forward_list<T> obj;
                    auto size = readSize(protocol);
                    auto reader = IReader<T>();
                    reader.checkType(protocol);
                    auto it = obj.before_begin();
                    for (decltype(size) i = 0; i < size; i++) {
                        it = obj.insert_after(it, std::move(reader(protocol)));
                    }
                    return obj;
                }
            };

            template<typename T>
            struct IReader<std::set<T>> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_SET == readType(protocol);
                }

                inline std::set<T> operator()(IProtocol &protocol) {
                    std::set<T> obj;
                    auto size = readSize(protocol);
                    auto reader = IReader<T>();
                    reader.readType(protocol);
                    for (decltype(size) i = 0; i < size; i++) {
                        obj.insert(std::move(reader(protocol)));
                    }
                    return obj;
                }
            };

            template<typename T>
            struct IReader<std::unordered_set<T>> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_SET == readType(protocol);
                }

                inline std::unordered_set<T> operator()(IProtocol &protocol) {
                    std::unordered_set<T> obj;
                    auto size = readSize(protocol);
                    auto reader = IReader<T>();
                    reader.readType(protocol);
                    obj.reserve(size);
                    for (decltype(size) i = 0; i < size; i++) {
                        obj.insert(std::move(reader(protocol)));
                    }
                    return obj;
                }
            };

            template<typename K, typename V>
            struct IReader<std::map<K, V>> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_MAP == readType(protocol);
                }

                inline std::map<K, V> operator()(IProtocol &protocol) {
                    std::map<K, V> obj;
                    auto size = readSize(protocol);
                    auto r_key = IReader<K>();
                    auto r_value = IReader<V>();
                    r_key.readType(protocol);
                    r_value.readType(protocol);
                    for (decltype(size) i = 0; i < size; i++) {
                        obj[std::move(IReader<K>()(protocol))] = std::move(IReader<V>()(protocol));
                    }
                    return obj;
                }
            };

            template<typename K, typename V>
            struct IReader<std::unordered_map<K, V>> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_MAP == readType(protocol);
                }

                inline std::unordered_map<K, V> operator()(IProtocol &protocol) {
                    std::unordered_map<K, V> obj;
                    auto size = readSize(protocol);
                    auto r_key = IReader<K>();
                    auto r_value = IReader<V>();
                    r_key.readType(protocol);
                    r_value.readType(protocol);
                    obj.reserve(size);
                    for (decltype(size) i = 0; i < size; i++) {
                        obj[std::move(IReader<K>()(protocol))] = std::move(IReader<V>()(protocol));
                    }
                    return obj;
                }
            };

            template<typename T1, typename T2>
            struct IReader<std::pair<T1, T2>> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_PAIR == readType(protocol);
                }

                inline std::pair<T1, T2> operator()(IProtocol &protocol) {
                    std::pair<T1, T2> obj;
                    auto r_first = IReader<T1>();
                    auto r_second = IReader<T1>();
                    r_first.readType(protocol);
                    r_second.readType(protocol);
                    obj.first = std::move(r_first(protocol));
                    obj.second = std::move(r_second(protocol));
                    return obj;
                }
            };

        }
    }
}

#endif
