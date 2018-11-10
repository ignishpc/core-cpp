
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
#include "../../exceptions/ILogicError.h"
#include "../RTTInfo.h"

namespace ignis {
    namespace data {
        namespace handle {

            typedef apache::thrift::protocol::TProtocol IProtocol;

            inline void writeSizeAux(IProtocol &protocol, int64_t size) {
                protocol.writeI64(size);
            }

            inline void writeTypeAux(IProtocol &protocol, int8_t size) {
                protocol.writeByte(size);
            }

            template<typename T>
            struct IWriterType {

                void writeType(IProtocol &protocol) {
                    throw exceptions::ILogicError(
                            "IWriterType not implemented for " + RTTInfo(typeid(T)).getStandardName());
                }

                void operator()(const T &obj, IProtocol &protocol) {
                    throw exceptions::ILogicError(
                            "IWriterType not implemented for " + RTTInfo(typeid(T)).getStandardName());
                }
            };

            template<>
            struct IWriterType<bool> {
                inline void writeType(IProtocol &protocol) {
                    writeTypeAux(protocol, IEnumTypes::I_BOOL);
                }

                inline void operator()(const bool &obj, IProtocol &protocol) {
                    protocol.writeBool(obj);
                }
            };

            template<>
            struct IWriterType<int8_t> {
                inline void writeType(IProtocol &protocol) {
                    writeTypeAux(protocol, IEnumTypes::I_I08);
                }

                inline void operator()(const int8_t &obj, IProtocol &protocol) {
                    protocol.writeByte(obj);
                }
            };

            template<>
            struct IWriterType<uint8_t> {
                inline void writeType(IProtocol &protocol) {
                    writeTypeAux(protocol, IEnumTypes::I_I16);
                }

                inline void operator()(const uint8_t &obj, IProtocol &protocol) {
                    protocol.writeI16((int16_t) obj);
                }
            };

            template<>
            struct IWriterType<int16_t> {
                inline void writeType(IProtocol &protocol) {
                    writeTypeAux(protocol, IEnumTypes::I_I16);
                }

                inline void operator()(const int16_t &obj, IProtocol &protocol) {
                    protocol.writeI16(obj);
                }
            };

            template<>
            struct IWriterType<uint16_t> {
                inline void writeType(IProtocol &protocol) {
                    writeTypeAux(protocol, IEnumTypes::I_I32);
                }

                inline void operator()(const uint16_t &obj, IProtocol &protocol) {
                    protocol.writeI32((int32_t) obj);
                }
            };

            template<>
            struct IWriterType<int32_t> {
                inline void writeType(IProtocol &protocol) {
                    writeTypeAux(protocol, IEnumTypes::I_I32);
                }

                inline void operator()(const int32_t &obj, IProtocol &protocol) {
                    protocol.writeI32(obj);
                }
            };

            template<>
            struct IWriterType<uint32_t> {
                inline void writeType(IProtocol &protocol) {
                    writeTypeAux(protocol, IEnumTypes::I_I64);
                }

                inline void operator()(const uint32_t &obj, IProtocol &protocol) {
                    protocol.writeI64((int64_t) obj);
                }
            };

            template<>
            struct IWriterType<int64_t> {
                inline void writeType(IProtocol &protocol) {
                    writeTypeAux(protocol, IEnumTypes::I_I64);
                }

                inline void operator()(const int64_t &obj, IProtocol &protocol) {
                    protocol.writeI64(obj);
                }
            };

            template<>
            struct IWriterType<uint64_t> {
                inline void writeType(IProtocol &protocol) {
                    writeTypeAux(protocol, IEnumTypes::I_PAIR);
                    writeTypeAux(protocol, IEnumTypes::I_BOOL);
                    writeTypeAux(protocol, IEnumTypes::I_I64);
                }

                inline void operator()(const uint64_t &obj, IProtocol &protocol) {
                    protocol.writeBool((obj >> 63) == 1);
                    protocol.writeI64((int64_t) (obj & 0x7FFFFFFFFFFFFFFF));
                }
            };

            template<>
            struct IWriterType<float> {
                inline void writeType(IProtocol &protocol) {
                    writeTypeAux(protocol, IEnumTypes::I_DOUBLE);
                }

                inline void operator()(const float &obj, IProtocol &protocol) {
                    protocol.writeDouble(obj);
                }
            };

            template<>
            struct IWriterType<double> {
                inline void writeType(IProtocol &protocol) {
                    writeTypeAux(protocol, IEnumTypes::I_DOUBLE);
                }

                inline void operator()(const double &obj, IProtocol &protocol) {
                    protocol.writeDouble(obj);
                }
            };

            template<>
            struct IWriterType<std::string> {
                inline void writeType(IProtocol &protocol) {
                    writeTypeAux(protocol, IEnumTypes::I_STRING);
                }

                inline void operator()(const std::string &obj, IProtocol &protocol) {
                    protocol.writeString(obj);
                }
            };

            template<typename _Tp, typename _Alloc>
            struct IWriterType<std::vector<_Tp, _Alloc>> {
                inline void writeType(IProtocol &protocol) {
                    writeTypeAux(protocol, IEnumTypes::I_LIST);
                }

                inline void operator()(const std::vector<_Tp, _Alloc> &obj, IProtocol &protocol) {
                    writeSizeAux(protocol, obj.size());
                    auto writer = IWriterType<_Tp>();
                    writer.writeType(protocol);
                    for (const auto &elem: obj) {
                        writer(elem, protocol);
                    }
                }
            };

            template<typename _T1, typename _T2, typename _Alloc>
            struct IWriterType<std::vector<std::pair<_T1, _T2>, _Alloc>> {
                inline void writeType(IProtocol &protocol) {
                    writeTypeAux(protocol, IEnumTypes::I_PAIR_LIST);
                }

                inline void operator()(const std::vector<std::pair<_T1, _T2>, _Alloc> &obj, IProtocol &protocol) {
                    writeSizeAux(protocol, obj.size());
                    auto w_first = IWriterType<_T1>();
                    auto w_second = IWriterType<_T2>();
                    w_first.writeType(protocol);
                    w_second.writeType(protocol);
                    for (auto &elem: obj) {
                        w_first(elem.first, protocol);
                        w_second(elem.second, protocol);
                    }
                }
            };

            template<typename _Alloc>
            struct IWriterType<std::vector<uint8_t, _Alloc>> {
                inline void writeType(IProtocol &protocol) {
                    writeTypeAux(protocol, IEnumTypes::I_BINARY);
                }

                inline void operator()(const std::vector<uint8_t, _Alloc> &obj, IProtocol &protocol) {
                    auto size = obj.size();
                    writeSizeAux(protocol, size);
                    IWriterType<uint8_t>().writeType(protocol);
                    auto data = obj.data();
                    for (decltype(size) i = 0; i < size; i++) {
                        protocol.writeByte(*(int8_t *) data);
                    }
                }
            };

            template<typename _Tp, typename _Alloc>
            struct IWriterType<std::list<_Tp, _Alloc>> {
                inline void writeType(IProtocol &protocol) {
                    writeTypeAux(protocol, IEnumTypes::I_LIST);
                }

                inline void operator()(const std::list<_Tp, _Alloc> &obj, IProtocol &protocol) {
                    writeSizeAux(protocol, obj.size());
                    auto writer = IWriterType<_Tp>();
                    writer.writeType(protocol);
                    for (auto it = obj.begin(); it != obj.end(); it++) {
                        writer(*it, protocol);
                    }
                }
            };

            template<typename _Tp, typename _Alloc>
            struct IWriterType<std::forward_list<_Tp, _Alloc>> {
                inline void writeType(IProtocol &protocol) {
                    writeTypeAux(protocol, IEnumTypes::I_LIST);
                }

                inline void operator()(const std::forward_list<_Tp, _Alloc> &obj, IProtocol &protocol) {
                    size_t size = 0;
                    for (auto it = obj.begin(); it != obj.end(); it++) {
                        size++;
                    }
                    writeSizeAux(protocol, size);
                    auto writer = IWriterType<_Tp>();
                    writer.writeType(protocol);
                    for (auto it = obj.begin(); it != obj.end(); it++) {
                        writer(*it, protocol);
                    }
                }
            };

            template<typename _Key, typename _Compare>
            struct IWriterType<std::set<_Key, _Compare>> {
                inline void writeType(IProtocol &protocol) {
                    writeTypeAux(protocol, IEnumTypes::I_SET);
                }

                inline void operator()(const std::set<_Key, _Compare> &obj, IProtocol &protocol) {
                    writeSizeAux(protocol, obj.size());
                    auto writer = IWriterType<_Key>();
                    writer.writeType(protocol);
                    for (auto it = obj.begin(); it != obj.end(); it++) {
                        writer(*it, protocol);
                    }
                }
            };


            template<typename _Value, typename _Hash, typename _Pred, typename _Alloc>
            struct IWriterType<std::unordered_set<_Value, _Hash, _Pred, _Alloc>> {
                inline void writeType(IProtocol &protocol) {
                    writeTypeAux(protocol, IEnumTypes::I_SET);
                }

                inline void operator()(const std::unordered_set<_Value, _Hash, _Pred, _Alloc> &obj, IProtocol &protocol) {
                    writeSizeAux(protocol, obj.size());
                    auto writer = IWriterType<_Value>();
                    writer.writeType(protocol);
                    for (auto it = obj.begin(); it != obj.end(); it++) {
                        writer(*it, protocol);
                    }
                }
            };

            template<typename _Key, typename _Tp, typename _Compare, typename _Alloc>
            struct IWriterType<std::map<_Key, _Tp, _Compare, _Alloc>> {
                inline void writeType(IProtocol &protocol) {
                    writeTypeAux(protocol, IEnumTypes::I_MAP);
                }

                inline void operator()(const std::map<_Key, _Tp, _Compare, _Alloc> &obj, IProtocol &protocol) {
                    writeSizeAux(protocol, obj.size());
                    auto w_key = IWriterType<_Key>();
                    auto w_value = IWriterType<_Tp>();
                    w_key.writeType(protocol);
                    w_value.writeType(protocol);
                    for (auto it = obj.begin(); it != obj.end(); it++) {
                        w_key(it->first, protocol);
                        w_value(it->second, protocol);
                    }
                }
            };

            template<typename _Key, typename _Tp, typename _Hash, typename _Pred , typename _Alloc>
            struct IWriterType<std::unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc>> {
                inline void writeType(IProtocol &protocol) {
                    writeTypeAux(protocol, IEnumTypes::I_MAP);
                }

                inline void operator()(const std::unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc> &obj, IProtocol &protocol) {
                    writeSizeAux(protocol, obj.size());
                    auto w_key = IWriterType<_Key>();
                    auto w_value = IWriterType<_Tp>();
                    w_key.writeType(protocol);
                    w_value.writeType(protocol);
                    for (auto it = obj.begin(); it != obj.end(); it++) {
                        w_key(it->first, protocol);
                        w_value(it->second, protocol);
                    }
                }
            };

            template<typename _T1, typename _T2>
            struct IWriterType<std::pair<_T1, _T2>> {
                inline void writeType(IProtocol &protocol) {
                    writeTypeAux(protocol, IEnumTypes::I_PAIR);
                }

                inline void operator()(const std::pair<_T1, _T2> &obj, IProtocol &protocol) {
                    auto w_first = IWriterType<_T1>();
                    auto w_second = IWriterType<_T2>();
                    w_first.writeType(protocol);
                    w_second.writeType(protocol);
                    w_first(obj.first, protocol);
                    w_second(obj.second, protocol);
                }
            };

            template<typename _Tp>
            struct IWriterType<_Tp *> {
                inline void writeType(IProtocol &protocol) {
                    IWriterType<_Tp>().writeType(protocol);
                }

                inline void operator()(const _Tp *&obj, IProtocol &protocol) {
                    IWriterType<_Tp>().operator()(*obj,protocol);
                }
            };

            template<typename _Tp>
            struct IWriterType<std::shared_ptr<_Tp>> {
                inline void writeType(IProtocol &protocol) {
                    IWriterType<_Tp>().writeType(protocol);
                }

                inline void operator()(const std::shared_ptr<_Tp> &obj, IProtocol &protocol) {
                    IWriterType<_Tp>().operator()(*obj,protocol);
                }
            };

            template<typename T>
            class IWriter {
            public:
                virtual void writeType(IProtocol &protocol) {
                    writer.writeType(protocol);
                }

                virtual void operator()(const T &obj, IProtocol &protocol) {
                    writer(obj, protocol);
                }

                virtual void writePtr(const T *obj, IProtocol &protocol) {
                    operator()(*obj, protocol);
                }

            private:
                IWriterType<T> writer;
            };
        }
    }
}

#endif
