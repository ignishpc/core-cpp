
#include "IWriter.h"

#include <string>
#include <list>
#include <forward_list>
#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <memory>

template<>
struct ignis::executor::core::io::IWriterType<bool> {
    inline void writeType(protocol::IProtocol &protocol) {
        writeTypeAux(protocol, IEnumTypes::I_BOOL);
    }

    inline void operator()(protocol::IProtocol &protocol, const bool &obj) {
        protocol.writeBool(obj);
    }
};

template<>
struct ignis::executor::core::io::IWriterType<int8_t> {
    inline void writeType(protocol::IProtocol &protocol) {
        writeTypeAux(protocol, IEnumTypes::I_I08);
    }

    inline void operator()(protocol::IProtocol &protocol, const int8_t &obj) {
        protocol.writeByte(obj);
    }
};

template<>
struct ignis::executor::core::io::IWriterType<uint8_t> {
    inline void writeType(protocol::IProtocol &protocol) {
        writeTypeAux(protocol, IEnumTypes::I_I08);
    }

    inline void operator()(protocol::IProtocol &protocol, const uint8_t &obj) {
        protocol.writeByte((int8_t &) obj);
    }
};

template<>
struct ignis::executor::core::io::IWriterType<int16_t> {
    inline void writeType(protocol::IProtocol &protocol) {
        writeTypeAux(protocol, IEnumTypes::I_I16);
    }

    inline void operator()(protocol::IProtocol &protocol, const int16_t &obj) {
        protocol.writeI16(obj);
    }
};

template<>
struct ignis::executor::core::io::IWriterType<uint16_t> {
    inline void writeType(protocol::IProtocol &protocol) {
        writeTypeAux(protocol, IEnumTypes::I_I32);
    }

    inline void operator()(protocol::IProtocol &protocol, const uint16_t &obj) {
        protocol.writeI16((int16_t &) obj);
    }
};

template<>
struct ignis::executor::core::io::IWriterType<int32_t> {
    inline void writeType(protocol::IProtocol &protocol) {
        writeTypeAux(protocol, IEnumTypes::I_I32);
    }

    inline void operator()(protocol::IProtocol &protocol, const int32_t &obj) {
        protocol.writeI32(obj);
    }
};

template<>
struct ignis::executor::core::io::IWriterType<uint32_t> {
    inline void writeType(protocol::IProtocol &protocol) {
        writeTypeAux(protocol, IEnumTypes::I_I64);
    }

    inline void operator()(protocol::IProtocol &protocol, const uint32_t &obj) {
        protocol.writeI32((int32_t &) obj);
    }
};

template<>
struct ignis::executor::core::io::IWriterType<int64_t> {
    inline void writeType(protocol::IProtocol &protocol) {
        writeTypeAux(protocol, IEnumTypes::I_I64);
    }

    inline void operator()(protocol::IProtocol &protocol, const int64_t &obj) {
        protocol.writeI64(obj);
    }
};

template<>
struct ignis::executor::core::io::IWriterType<uint64_t> {
    inline void writeType(protocol::IProtocol &protocol) {
        writeTypeAux(protocol, IEnumTypes::I_I64);
    }

    inline void operator()(protocol::IProtocol &protocol, const uint64_t &obj) {
        protocol.writeI64((int64_t &) obj);
    }
};

template<>
struct ignis::executor::core::io::IWriterType<float> {
    inline void writeType(protocol::IProtocol &protocol) {
        writeTypeAux(protocol, IEnumTypes::I_DOUBLE);
    }

    inline void operator()(protocol::IProtocol &protocol, const float &obj) {
        protocol.writeDouble(obj);
    }
};

template<>
struct ignis::executor::core::io::IWriterType<double> {
    inline void writeType(protocol::IProtocol &protocol) {
        writeTypeAux(protocol, IEnumTypes::I_DOUBLE);
    }

    inline void operator()(protocol::IProtocol &protocol, const double &obj) {
        protocol.writeDouble(obj);
    }
};

template<>
struct ignis::executor::core::io::IWriterType<std::string> {
    inline void writeType(protocol::IProtocol &protocol) {
        writeTypeAux(protocol, IEnumTypes::I_STRING);
    }

    inline void operator()(protocol::IProtocol &protocol, const std::string &obj) {
        protocol.writeString(obj);
    }
};

template<typename _Tp, typename _Alloc>
struct ignis::executor::core::io::IWriterType<std::vector<_Tp, _Alloc>> {
    inline void writeType(protocol::IProtocol &protocol) {
        writeTypeAux(protocol, IEnumTypes::I_LIST);
    }

    inline void operator()(protocol::IProtocol &protocol, const std::vector<_Tp, _Alloc> &obj) {
        writeSizeAux(protocol, obj.size());
        auto writer = IWriterType<_Tp>();
        writer.writeType(protocol);
        for (const auto &elem: obj) {
            writer(protocol, elem);
        }
    }
};

template<typename _Alloc>
struct ignis::executor::core::io::IWriterType<std::vector<bool, _Alloc>> {
    inline void writeType(protocol::IProtocol &protocol) {
        writeTypeAux(protocol, IEnumTypes::I_LIST);
    }

    inline void operator()(protocol::IProtocol &protocol, const std::vector<bool, _Alloc> &obj) {
        writeSizeAux(protocol, obj.size());
        auto writer = IWriterType<bool>();
        writer.writeType(protocol);
        for (const bool elem: obj) {
            writer(protocol, elem);
        }
    }
};

template<typename _T1, typename _T2, typename _Alloc>
struct ignis::executor::core::io::IWriterType<std::vector<std::pair<_T1, _T2>, _Alloc>> {
    inline void writeType(protocol::IProtocol &protocol) {
        writeTypeAux(protocol, IEnumTypes::I_PAIR_LIST);
    }

    inline void
    operator()(protocol::IProtocol &protocol, const std::vector<std::pair<_T1, _T2>, _Alloc> &obj) {
        writeSizeAux(protocol, obj.size());
        auto w_first = IWriterType<_T1>();
        auto w_second = IWriterType<_T2>();
        w_first.writeType(protocol);
        w_second.writeType(protocol);
        for (auto &elem: obj) {
            w_first(protocol, elem.first);
            w_second(protocol, elem.second);
        }
    }
};

template<typename _Alloc>
struct ignis::executor::core::io::IWriterType<std::vector<uint8_t, _Alloc>> {
    inline void writeType(protocol::IProtocol &protocol) {
        writeTypeAux(protocol, IEnumTypes::I_BINARY);
    }

    inline void operator()(protocol::IProtocol &protocol, const std::vector<uint8_t, _Alloc> &obj) {
        auto size = obj.size();
        writeSizeAux(protocol, size);
        auto data = obj.data();
        for (decltype(size) i = 0; i < size; i++) {
            protocol.writeByte((int8_t &) data[i]);
        }
    }
};

template<typename _Tp, typename _Alloc>
struct ignis::executor::core::io::IWriterType<std::list<_Tp, _Alloc>> {
    inline void writeType(protocol::IProtocol &protocol) {
        writeTypeAux(protocol, IEnumTypes::I_LIST);
    }

    inline void operator()(protocol::IProtocol &protocol, const std::list<_Tp, _Alloc> &obj) {
        writeSizeAux(protocol, obj.size());
        auto writer = IWriterType<_Tp>();
        writer.writeType(protocol);
        for (auto it = obj.begin(); it != obj.end(); it++) {
            writer(protocol, *it);
        }
    }
};

template<typename _Tp, typename _Alloc>
struct ignis::executor::core::io::IWriterType<std::forward_list<_Tp, _Alloc>> {
    inline void writeType(protocol::IProtocol &protocol) {
        writeTypeAux(protocol, IEnumTypes::I_LIST);
    }

    inline void operator()(protocol::IProtocol &protocol, const std::forward_list<_Tp, _Alloc> &obj) {
        size_t size = 0;
        for (auto it = obj.begin(); it != obj.end(); it++) {
            size++;
        }
        writeSizeAux(protocol, size);
        auto writer = IWriterType<_Tp>();
        writer.writeType(protocol);
        for (auto it = obj.begin(); it != obj.end(); it++) {
            writer(protocol, *it);
        }
    }
};

template<typename _Key, typename _Compare, typename _Alloc>
struct ignis::executor::core::io::IWriterType<std::set<_Key, _Compare, _Alloc>> {
    inline void writeType(protocol::IProtocol &protocol) {
        writeTypeAux(protocol, IEnumTypes::I_SET);
    }

    inline void operator()(protocol::IProtocol &protocol, const std::set<_Key, _Compare, _Alloc> &obj) {
        writeSizeAux(protocol, obj.size());
        auto writer = IWriterType<_Key>();
        writer.writeType(protocol);
        for (auto it = obj.begin(); it != obj.end(); it++) {
            writer(protocol, *it);
        }
    }
};


template<typename _Value, typename _Hash, typename _Pred, typename _Alloc>
struct ignis::executor::core::io::IWriterType<std::unordered_set<_Value, _Hash, _Pred, _Alloc>> {
    inline void writeType(protocol::IProtocol &protocol) {
        writeTypeAux(protocol, IEnumTypes::I_SET);
    }

    inline void
    operator()(protocol::IProtocol &protocol, const std::unordered_set<_Value, _Hash, _Pred, _Alloc> &obj) {
        writeSizeAux(protocol, obj.size());
        auto writer = IWriterType<_Value>();
        writer.writeType(protocol);
        for (auto it = obj.begin(); it != obj.end(); it++) {
            writer(protocol, *it);
        }
    }
};

template<typename _Key, typename _Tp, typename _Compare, typename _Alloc>
struct ignis::executor::core::io::IWriterType<std::map<_Key, _Tp, _Compare, _Alloc>> {
    inline void writeType(protocol::IProtocol &protocol) {
        writeTypeAux(protocol, IEnumTypes::I_MAP);
    }

    inline void operator()(protocol::IProtocol &protocol, const std::map<_Key, _Tp, _Compare, _Alloc> &obj) {
        writeSizeAux(protocol, obj.size());
        auto w_key = IWriterType<_Key>();
        auto w_value = IWriterType<_Tp>();
        w_key.writeType(protocol);
        w_value.writeType(protocol);
        for (auto it = obj.begin(); it != obj.end(); it++) {
            w_key(protocol, it->first);
            w_value(protocol, it->second);
        }
    }
};

template<typename _Key, typename _Tp, typename _Hash, typename _Pred, typename _Alloc>
struct ignis::executor::core::io::IWriterType<std::unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc>> {
    inline void writeType(protocol::IProtocol &protocol) {
        writeTypeAux(protocol, IEnumTypes::I_MAP);
    }

    inline void
    operator()(protocol::IProtocol &protocol, const std::unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc> &obj) {
        writeSizeAux(protocol, obj.size());
        auto w_key = IWriterType<_Key>();
        auto w_value = IWriterType<_Tp>();
        w_key.writeType(protocol);
        w_value.writeType(protocol);
        for (auto it = obj.begin(); it != obj.end(); it++) {
            w_key(protocol, it->first);
            w_value(protocol, it->second);
        }
    }
};

template<typename _T1, typename _T2>
struct ignis::executor::core::io::IWriterType<std::pair<_T1, _T2>> {
    inline void writeType(protocol::IProtocol &protocol) {
        writeTypeAux(protocol, IEnumTypes::I_PAIR);
        first_writer.writeType(protocol);
        second_writer.writeType(protocol);
    }

    inline void operator()(protocol::IProtocol &protocol, const std::pair<_T1, _T2> &obj) {
        first_writer(protocol, obj.first);
        second_writer(protocol, obj.second);
    }

private:
    IWriterType <_T1> first_writer;
    IWriterType <_T2> second_writer;
};

template<typename _Tp>
struct ignis::executor::core::io::IWriterType<_Tp *> {
    inline void writeType(protocol::IProtocol &protocol) {
        writer.writeType(protocol);
    }

    inline void operator()(protocol::IProtocol &protocol, const _Tp *&obj) {
        writer(protocol, *obj);
    }

private:
    IWriterType <_Tp> writer;
};

template<typename _Tp>
struct ignis::executor::core::io::IWriterType<std::shared_ptr<_Tp>> {
    inline void writeType(protocol::IProtocol &protocol) {
        writer.writeType(protocol);
    }

    inline void operator()(protocol::IProtocol &protocol, const std::shared_ptr<_Tp> &obj) {
        writer(protocol, *obj);
    }

private:
    IWriterType <_Tp> writer;
};
