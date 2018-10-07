
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
#include "../../exceptions/ILogicError.h"
#include "../RTTInfo.h"

namespace ignis {
    namespace data {
        namespace handle {

            typedef apache::thrift::protocol::TProtocol IProtocol;

            inline int8_t readTypeAux(IProtocol &protocol) {
                int8_t type;
                protocol.readByte(type);
                return type;
            }

            inline int64_t readSizeAux(IProtocol &protocol) {
                int64_t size;
                protocol.readI64(size);
                return size;
            }

            template <typename T>
            inline void checkTypeAux(bool readType) {
                if (readType) {
                    throw exceptions::ILogicError(
                            "IReaderType type error for" + RTTInfo(typeid(T)).getStandardName());
                }
            }

            template<typename T>
            struct IReaderType {

                virtual bool readType(IProtocol &protocol) {
                    throw exceptions::ILogicError(
                            "IReaderType not implemented for " + RTTInfo(typeid(T)).getStandardName());
                }

                virtual T operator()(IProtocol &protocol) {
                    throw exceptions::ILogicError(
                            "IReaderType not implemented for " + RTTInfo(typeid(T)).getStandardName());
                }
            };

            template<>
            struct IReaderType<bool> {
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
            struct IReaderType<int8_t> {
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
            struct IReaderType<uint8_t> {
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
            struct IReaderType<int16_t> {
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
            struct IReaderType<uint16_t> {
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
            struct IReaderType<int32_t> {
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
            struct IReaderType<uint32_t> {
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
            struct IReaderType<int64_t> {
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
            struct IReaderType<uint64_t> {
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
            struct IReaderType<float> {
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
            struct IReaderType<double> {
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
            struct IReaderType<std::string> {
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
            struct IReaderType<std::vector<T>> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_LIST == readType(protocol);
                }

                inline std::vector<T> operator()(IProtocol &protocol) {
                    std::vector<T> obj;
                    auto size = readSizeAux(protocol);
                    auto reader = IReaderType<T>();
                    checkTypeAux<T>(reader.readType(protocol));
                    obj.reserve(size);
                    for (decltype(size) i = 0; i < size; i++) {
                        obj.push_back(reader(protocol));
                    }
                    return obj;
                }
            };

            template<>
            struct IReaderType<std::vector<uint8_t >> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_BINARY == readType(protocol);
                }

                inline std::vector<uint8_t> operator()(IProtocol &protocol) {
                    std::vector<uint8_t> obj;
                    auto size = readSizeAux(protocol);
                    obj.resize(size);
                    auto data = obj.data();
                    for (decltype(size) i = 0; i < size; i++) {
                        protocol.readByte(*(int8_t *) data);
                    }
                    return obj;
                }
            };

            template<typename T>
            struct IReaderType<std::list<T>> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_LIST == readType(protocol);
                }

                inline std::list<T> operator()(IProtocol &protocol) {
                    std::list<T> obj;
                    auto size = readSizeAux(protocol);
                    auto reader = IReaderType<T>();
                    checkTypeAux<T>(reader.readType(protocol));
                    for (decltype(size) i = 0; i < size; i++) {
                        obj.push_back(std::move(reader(protocol)));
                    }
                    return obj;
                }
            };

            template<typename T>
            struct IReaderType<std::forward_list<T>> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_LIST == readType(protocol);
                }

                inline std::forward_list<T> operator()(IProtocol &protocol) {
                    std::forward_list<T> obj;
                    auto size = readSizeAux(protocol);
                    auto reader = IReaderType<T>();
                    checkTypeAux<T>(reader.readType(protocol));
                    auto it = obj.before_begin();
                    for (decltype(size) i = 0; i < size; i++) {
                        it = obj.insert_after(it, std::move(reader(protocol)));
                    }
                    return obj;
                }
            };

            template<typename T>
            struct IReaderType<std::set<T>> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_SET == readType(protocol);
                }

                inline std::set<T> operator()(IProtocol &protocol) {
                    std::set<T> obj;
                    auto size = readSizeAux(protocol);
                    auto reader = IReaderType<T>();
                    checkTypeAux<T>(reader.readType(protocol));
                    for (decltype(size) i = 0; i < size; i++) {
                        obj.insert(std::move(reader(protocol)));
                    }
                    return obj;
                }
            };

            template<typename T>
            struct IReaderType<std::unordered_set<T>> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_SET == readType(protocol);
                }

                inline std::unordered_set<T> operator()(IProtocol &protocol) {
                    std::unordered_set<T> obj;
                    auto size = readSizeAux(protocol);
                    auto reader = IReaderType<T>();
                    checkTypeAux<T>(reader.readType(protocol));
                    obj.reserve(size);
                    for (decltype(size) i = 0; i < size; i++) {
                        obj.insert(std::move(reader(protocol)));
                    }
                    return obj;
                }
            };

            template<typename K, typename V>
            struct IReaderType<std::map<K, V>> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_MAP == readType(protocol);
                }

                inline std::map<K, V> operator()(IProtocol &protocol) {
                    std::map<K, V> obj;
                    auto size = readSizeAux(protocol);
                    auto r_key = IReaderType<K>();
                    auto r_value = IReaderType<V>();
                    checkTypeAux<K>(r_key.readType(protocol));
                    checkTypeAux<V>(r_value.readType(protocol));
                    for (decltype(size) i = 0; i < size; i++) {
                        obj[std::move(IReaderType<K>()(protocol))] = std::move(IReaderType<V>()(protocol));
                    }
                    return obj;
                }
            };

            template<typename K, typename V>
            struct IReaderType<std::unordered_map<K, V>> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_MAP == readType(protocol);
                }

                inline std::unordered_map<K, V> operator()(IProtocol &protocol) {
                    std::unordered_map<K, V> obj;
                    auto size = readSizeAux(protocol);
                    auto r_key = IReaderType<K>();
                    auto r_value = IReaderType<V>();
                    checkTypeAux<K>(r_key.readType(protocol));
                    checkTypeAux<V>(r_value.readType(protocol));
                    obj.reserve(size);
                    for (decltype(size) i = 0; i < size; i++) {
                        obj[std::move(IReaderType<K>()(protocol))] = std::move(IReaderType<V>()(protocol));
                    }
                    return obj;
                }
            };

            template<typename T1, typename T2>
            struct IReaderType<std::pair<T1, T2>> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_PAIR == readType(protocol);
                }

                inline std::pair<T1, T2> operator()(IProtocol &protocol) {
                    std::pair<T1, T2> obj;
                    auto r_first = IReaderType<T1>();
                    auto r_second = IReaderType<T2>();
                    checkTypeAux<T1>(r_first.readType(protocol));
                    checkTypeAux<T2>(r_second.readType(protocol));
                    obj.first = r_first(protocol);
                    obj.second = r_second(protocol);
                    return obj;
                }
            };

            template<typename T>
            class IReader {
            public:
                virtual inline std::shared_ptr<T> readShared(IProtocol &protocol) {
                    readType(protocol);
                    return std::make_shared<T>(reader(protocol));
                }

                virtual inline T *readPtr(IProtocol &protocol) {
                    readType(protocol);
                    return new T(reader(protocol));
                }

                virtual T operator()(IProtocol &protocol) {
                    readType(protocol);
                    return reader(protocol);
                }

                virtual inline void readType(IProtocol &protocol) {
                    checkTypeAux<T>(reader.readType(protocol));
                }

            private:
                IReaderType<T> reader;

            };

        }
    }
}

#endif
