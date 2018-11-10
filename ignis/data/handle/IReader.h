
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

            template<typename T>
            inline void checkTypeAux(bool readType) {
                if (!readType) {
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
                    return IEnumTypes::I_BOOL == readTypeAux(protocol);
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
                    return IEnumTypes::I_I08 == readTypeAux(protocol);
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
                    return IEnumTypes::I_I16 == readTypeAux(protocol);
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
                    return IEnumTypes::I_I16 == readTypeAux(protocol);
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
                    return IEnumTypes::I_I32 == readTypeAux(protocol);
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
                    return IEnumTypes::I_I32 == readTypeAux(protocol);
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
                    return IEnumTypes::I_I64 == readTypeAux(protocol);
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
                    return IEnumTypes::I_I64 == readTypeAux(protocol);
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
                    return IEnumTypes::I_PAIR == readTypeAux(protocol) &&
                           IEnumTypes::I_BOOL == readTypeAux(protocol) &&
                           IEnumTypes::I_I64 == readTypeAux(protocol);
                }

                inline uint64_t operator()(IProtocol &protocol) {
                    bool flag;
                    uint64_t obj;
                    protocol.readBool(flag);
                    protocol.readI64((int64_t &) obj);
                    if(flag){
                        obj |= 1UL << 63;
                    }
                    return obj;
                }
            };

            template<>
            struct IReaderType<float> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_DOUBLE == readTypeAux(protocol);
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
                    return IEnumTypes::I_DOUBLE == readTypeAux(protocol);
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
                    return IEnumTypes::I_STRING == readTypeAux(protocol);
                }

                inline std::string operator()(IProtocol &protocol) {
                    std::string obj;
                    protocol.readString(obj);
                    return obj;
                }
            };

            template<typename _Tp, typename _Alloc>
            struct IReaderType<std::vector<_Tp, _Alloc>> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_LIST == readTypeAux(protocol);
                }

                inline std::vector<_Tp, _Alloc> operator()(IProtocol &protocol) {
                    std::vector<_Tp, _Alloc> obj;
                    auto size = readSizeAux(protocol);
                    auto reader = IReaderType<_Tp>();
                    checkTypeAux<_Tp>(reader.readType(protocol));
                    obj.reserve(size);
                    for (decltype(size) i = 0; i < size; i++) {
                        obj.push_back(reader(protocol));
                    }
                    return obj;
                }
            };

            template<typename _T1, typename _T2, typename _Alloc>
            struct IReaderType<std::vector<std::pair<_T1, _T2>, _Alloc>> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_PAIR_LIST == readTypeAux(protocol);
                }

                inline std::vector<std::pair<_T1, _T2>, _Alloc> operator()(IProtocol &protocol) {
                    std::vector<std::pair<_T1, _T2>, _Alloc> obj;
                    auto size = readSizeAux(protocol);
                    obj.reserve(size);
                    auto r_first = IReaderType<_T1>();
                    auto r_second = IReaderType<_T2>();
                    checkTypeAux<_T1>(r_first.readType(protocol));
                    checkTypeAux<_T2>(r_second.readType(protocol));
                    for (decltype(size) i = 0; i < size; i++) {
                        obj.push_back(std::make_pair<_T1, _T2>(r_first(protocol), r_second(protocol)));
                    }
                    return obj;
                }
            };

            template<typename _Alloc>
            struct IReaderType<std::vector<uint8_t, _Alloc>> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_BINARY == readTypeAux(protocol);
                }

                inline std::vector<uint8_t, _Alloc> operator()(IProtocol &protocol) {
                    std::vector<uint8_t, _Alloc> obj;
                    auto size = readSizeAux(protocol);
                    obj.resize(size);
                    auto data = obj.data();
                    for (decltype(size) i = 0; i < size; i++) {
                        protocol.readByte(*(int8_t *) data);
                    }
                    return obj;
                }
            };

            template<typename _Tp, typename _Alloc>
            struct IReaderType<std::list<_Tp, _Alloc>> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_LIST == readTypeAux(protocol);
                }

                inline std::list<_Tp, _Alloc> operator()(IProtocol &protocol) {
                    std::list<_Tp, _Alloc> obj;
                    auto size = readSizeAux(protocol);
                    auto reader = IReaderType<_Tp>();
                    checkTypeAux<_Tp>(reader.readType(protocol));
                    for (decltype(size) i = 0; i < size; i++) {
                        obj.push_back(std::move(reader(protocol)));
                    }
                    return obj;
                }
            };

            template<typename _Tp, typename _Alloc>
            struct IReaderType<std::forward_list<_Tp, _Alloc>> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_LIST == readTypeAux(protocol);
                }

                inline std::forward_list<_Tp, _Alloc> operator()(IProtocol &protocol) {
                    std::forward_list<_Tp, _Alloc> obj;
                    auto size = readSizeAux(protocol);
                    auto reader = IReaderType<_Tp>();
                    checkTypeAux<_Tp>(reader.readType(protocol));
                    auto it = obj.before_begin();
                    for (decltype(size) i = 0; i < size; i++) {
                        it = obj.insert_after(it, std::move(reader(protocol)));
                    }
                    return obj;
                }
            };

            template<typename _Key, typename _Compare>
            struct IReaderType<std::set<_Key, _Compare>> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_SET == readTypeAux(protocol);
                }

                inline std::set<_Key, _Compare> operator()(IProtocol &protocol) {
                    std::set<_Key, _Compare> obj;
                    auto size = readSizeAux(protocol);
                    auto reader = IReaderType<_Key>();
                    checkTypeAux<_Key>(reader.readType(protocol));
                    for (decltype(size) i = 0; i < size; i++) {
                        obj.insert(std::move(reader(protocol)));
                    }
                    return obj;
                }
            };

            template<typename _Value, typename _Hash, typename _Pred, typename _Alloc>
            struct IReaderType<std::unordered_set<_Value, _Hash, _Pred, _Alloc>> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_SET == readTypeAux(protocol);
                }

                inline std::unordered_set<_Value, _Hash, _Pred, _Alloc> operator()(IProtocol &protocol) {
                    std::unordered_set<_Value, _Hash, _Pred, _Alloc> obj;
                    auto size = readSizeAux(protocol);
                    auto reader = IReaderType<_Value>();
                    checkTypeAux<_Value>(reader.readType(protocol));
                    obj.reserve(size);
                    for (decltype(size) i = 0; i < size; i++) {
                        obj.insert(std::move(reader(protocol)));
                    }
                    return obj;
                }
            };

            template<typename _Key, typename _Tp, typename _Compare, typename _Alloc>
            struct IReaderType<std::map<_Key, _Tp, _Compare, _Alloc>> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_MAP == readTypeAux(protocol);
                }

                inline std::map<_Key, _Tp, _Compare, _Alloc> operator()(IProtocol &protocol) {
                    std::map<_Key, _Tp, _Compare, _Alloc> obj;
                    auto size = readSizeAux(protocol);
                    auto r_key = IReaderType<_Key>();
                    auto r_value = IReaderType<_Tp>();
                    checkTypeAux<_Key>(r_key.readType(protocol));
                    checkTypeAux<_Tp>(r_value.readType(protocol));
                    for (decltype(size) i = 0; i < size; i++) {
                        obj[std::move(IReaderType<_Key>()(protocol))] = std::move(IReaderType<_Tp>()(protocol));
                    }
                    return obj;
                }
            };

            template<typename _Key, typename _Tp, typename _Hash, typename _Pred, typename _Alloc>
            struct IReaderType<std::unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc>> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_MAP == readTypeAux(protocol);
                }

                inline std::unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc> operator()(IProtocol &protocol) {
                    std::unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc> obj;
                    auto size = readSizeAux(protocol);
                    auto r_key = IReaderType<_Key>();
                    auto r_value = IReaderType<_Tp>();
                    checkTypeAux<_Key>(r_key.readType(protocol));
                    checkTypeAux<_Tp>(r_value.readType(protocol));
                    obj.reserve(size);
                    for (decltype(size) i = 0; i < size; i++) {
                        obj[std::move(IReaderType<_Key>()(protocol))] = std::move(IReaderType<_Tp>()(protocol));
                    }
                    return obj;
                }
            };

            template<typename _T1, typename _T2>
            struct IReaderType<std::pair<_T1, _T2>> {
                inline bool readType(IProtocol &protocol) {
                    return IEnumTypes::I_PAIR == readTypeAux(protocol);
                }

                inline std::pair<_T1, _T2> operator()(IProtocol &protocol) {
                    std::pair<_T1, _T2> obj;
                    auto r_first = IReaderType<_T1>();
                    auto r_second = IReaderType<_T2>();
                    checkTypeAux<_T1>(r_first.readType(protocol));
                    checkTypeAux<_T2>(r_second.readType(protocol));
                    obj.first = r_first(protocol);
                    obj.second = r_second(protocol);
                    return obj;
                }
            };

            template<typename T>
            class IReader {
            public:
                virtual inline std::shared_ptr<T> readShared(IProtocol &protocol) {
                    return std::make_shared<T>(reader(protocol));
                }

                virtual inline T *readPtr(IProtocol &protocol) {
                    return new T(reader(protocol));
                }

                virtual T operator()(IProtocol &protocol) {
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
