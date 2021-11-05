
#include "IReader.h"

#include <forward_list>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>


template<>
struct ignis::executor::core::io::IReaderType<bool> {
    inline bool readType(protocol::IProtocol &protocol) { return IEnumTypes::I_BOOL == readTypeAux(protocol); }

    inline void operator()(protocol::IProtocol &protocol, bool &obj) { protocol.readBool(obj); }

    inline bool operator()(protocol::IProtocol &protocol) {
        bool obj;
        (*this)(protocol, obj);
        return obj;
    }
};

template<>
struct ignis::executor::core::io::IReaderType<int8_t> {
    inline bool readType(protocol::IProtocol &protocol) { return IEnumTypes::I_I08 == readTypeAux(protocol); }

    inline void operator()(protocol::IProtocol &protocol, int8_t &obj) { protocol.readByte(obj); }

    inline int8_t operator()(protocol::IProtocol &protocol) {
        int8_t obj;
        (*this)(protocol, obj);
        return obj;
    }
};

template<>
struct ignis::executor::core::io::IReaderType<uint8_t> {
    inline bool readType(protocol::IProtocol &protocol) { return IEnumTypes::I_I08 == readTypeAux(protocol); }

    inline void operator()(protocol::IProtocol &protocol, uint8_t &obj) { protocol.readByte((int8_t &) obj); }

    inline uint8_t operator()(protocol::IProtocol &protocol) {
        uint8_t obj;
        (*this)(protocol, obj);
        return obj;
    }
};

template<>
struct ignis::executor::core::io::IReaderType<int16_t> {
    inline bool readType(protocol::IProtocol &protocol) { return IEnumTypes::I_I16 == readTypeAux(protocol); }

    inline void operator()(protocol::IProtocol &protocol, int16_t &obj) { protocol.readI16(obj); }

    inline int16_t operator()(protocol::IProtocol &protocol) {
        int16_t obj;
        (*this)(protocol, obj);
        return obj;
    }
};

template<>
struct ignis::executor::core::io::IReaderType<uint16_t> {
    inline bool readType(protocol::IProtocol &protocol) { return IEnumTypes::I_I16 == readTypeAux(protocol); }

    inline void operator()(protocol::IProtocol &protocol, uint16_t &obj) { protocol.readI16((int16_t &) obj); }

    inline uint16_t operator()(protocol::IProtocol &protocol) {
        uint16_t obj;
        (*this)(protocol, obj);
        return obj;
    }
};

template<>
struct ignis::executor::core::io::IReaderType<int32_t> {
    inline bool readType(protocol::IProtocol &protocol) { return IEnumTypes::I_I32 == readTypeAux(protocol); }

    inline void operator()(protocol::IProtocol &protocol, int32_t &obj) { protocol.readI32(obj); }

    inline int32_t operator()(protocol::IProtocol &protocol) {
        int32_t obj;
        (*this)(protocol, obj);
        return obj;
    }
};

template<>
struct ignis::executor::core::io::IReaderType<uint32_t> {
    inline bool readType(protocol::IProtocol &protocol) { return IEnumTypes::I_I32 == readTypeAux(protocol); }

    inline void operator()(protocol::IProtocol &protocol, uint32_t &obj) { protocol.readI32((int32_t &) obj); }

    inline uint32_t operator()(protocol::IProtocol &protocol) {
        uint32_t obj;
        (*this)(protocol, obj);
        return obj;
    }
};

template<>
struct ignis::executor::core::io::IReaderType<int64_t> {
    inline bool readType(protocol::IProtocol &protocol) { return IEnumTypes::I_I64 == readTypeAux(protocol); }

    inline void operator()(protocol::IProtocol &protocol, int64_t &obj) { protocol.readI64(obj); }

    inline int64_t operator()(protocol::IProtocol &protocol) {
        int64_t obj;
        (*this)(protocol, obj);
        return obj;
    }
};

template<>
struct ignis::executor::core::io::IReaderType<uint64_t> {
    inline bool readType(protocol::IProtocol &protocol) { return IEnumTypes::I_I64 == readTypeAux(protocol); }

    inline void operator()(protocol::IProtocol &protocol, uint64_t &obj) { protocol.readI64((int64_t &) obj); }

    inline uint64_t operator()(protocol::IProtocol &protocol) {
        uint64_t obj;
        (*this)(protocol, obj);
        return obj;
    }
};

template<>
struct ignis::executor::core::io::IReaderType<float> {
    inline bool readType(protocol::IProtocol &protocol) { return IEnumTypes::I_DOUBLE == readTypeAux(protocol); }

    inline void operator()(protocol::IProtocol &protocol, double &obj) { protocol.readDouble(obj); }

    inline float operator()(protocol::IProtocol &protocol) {
        double obj;
        (*this)(protocol, obj);
        return (float) obj;
    }
};

template<>
struct ignis::executor::core::io::IReaderType<double> {
    inline bool readType(protocol::IProtocol &protocol) { return IEnumTypes::I_DOUBLE == readTypeAux(protocol); }

    inline void operator()(protocol::IProtocol &protocol, double &obj) { protocol.readDouble(obj); }

    inline double operator()(protocol::IProtocol &protocol) {
        double obj;
        (*this)(protocol, obj);
        return obj;
    }
};

template<>
struct ignis::executor::core::io::IReaderType<std::string> {
    inline bool readType(protocol::IProtocol &protocol) { return IEnumTypes::I_STRING == readTypeAux(protocol); }

    inline void operator()(protocol::IProtocol &protocol, std::string &obj) { protocol.readString(obj); }

    inline std::string operator()(protocol::IProtocol &protocol) {
        std::string obj;
        (*this)(protocol, obj);
        return std::move(obj);
    }
};

template<typename _Tp, typename _Alloc>
struct ignis::executor::core::io::IReaderType<std::vector<_Tp, _Alloc>> {
    inline bool readType(protocol::IProtocol &protocol) { return IEnumTypes::I_LIST == readTypeAux(protocol); }

    inline void operator()(protocol::IProtocol &protocol, std::vector<_Tp, _Alloc> &obj) {
        auto size = readSizeAux(protocol);
        obj.reserve(obj.size() + size);
        auto reader = IReaderType<_Tp>();
        checkTypeAux<_Tp>(reader.readType(protocol));
        for (decltype(size) i = 0; i < size; i++) { obj.push_back(reader(protocol)); }
    }

    inline std::vector<_Tp, _Alloc> operator()(protocol::IProtocol &protocol) {
        std::vector<_Tp, _Alloc> obj;
        (*this)(protocol, obj);
        return std::move(obj);
    }
};

template<typename _T1, typename _T2, typename _Alloc>
struct ignis::executor::core::io::IReaderType<std::vector<std::pair<_T1, _T2>, _Alloc>> {
    inline bool readType(protocol::IProtocol &protocol) { return IEnumTypes::I_PAIR_LIST == readTypeAux(protocol); }

    inline void operator()(protocol::IProtocol &protocol, std::vector<std::pair<_T1, _T2>, _Alloc> &obj) {
        auto size = readSizeAux(protocol);
        obj.reserve(obj.size() + size);
        auto r_first = IReaderType<_T1>();
        auto r_second = IReaderType<_T2>();
        checkTypeAux<_T1>(r_first.readType(protocol));
        checkTypeAux<_T2>(r_second.readType(protocol));
        for (decltype(size) i = 0; i < size; i++) {
            auto first = r_first(protocol);
            obj.emplace_back(std::move(first), r_second(protocol));
        }
    }

    inline std::vector<std::pair<_T1, _T2>, _Alloc> operator()(protocol::IProtocol &protocol) {
        std::vector<std::pair<_T1, _T2>, _Alloc> obj;
        (*this)(protocol, obj);
        return std::move(obj);
    }
};

template<typename _Alloc>
struct ignis::executor::core::io::IReaderType<std::vector<uint8_t, _Alloc>> {
    inline bool readType(protocol::IProtocol &protocol) { return IEnumTypes::I_BINARY == readTypeAux(protocol); }

    inline void operator()(protocol::IProtocol &protocol, std::vector<uint8_t, _Alloc> &obj) {
        auto size = readSizeAux(protocol);
        auto init = obj.size();
        obj.resize(init + size);
        auto data = obj.data() + init;
        for (decltype(size) i = 0; i < size; i++) { protocol.readByte((int8_t &) data[i]); }
    }

    inline std::vector<uint8_t, _Alloc> operator()(protocol::IProtocol &protocol) {
        std::vector<uint8_t, _Alloc> obj;
        (*this)(protocol, obj);
        return std::move(obj);
    }
};

template<typename _Tp, typename _Alloc>
struct ignis::executor::core::io::IReaderType<std::list<_Tp, _Alloc>> {
    inline bool readType(protocol::IProtocol &protocol) { return IEnumTypes::I_LIST == readTypeAux(protocol); }

    inline void operator()(protocol::IProtocol &protocol, std::list<_Tp, _Alloc> &obj) {
        auto size = readSizeAux(protocol);
        auto reader = IReaderType<_Tp>();
        checkTypeAux<_Tp>(reader.readType(protocol));
        for (decltype(size) i = 0; i < size; i++) { obj.push_back(std::move(reader(protocol))); }
    }

    inline std::list<_Tp, _Alloc> operator()(protocol::IProtocol &protocol) {
        std::list<_Tp, _Alloc> obj;
        (*this)(protocol, obj);
        return std::move(obj);
    }
};

template<typename _Tp, typename _Alloc>
struct ignis::executor::core::io::IReaderType<std::forward_list<_Tp, _Alloc>> {
    inline bool readType(protocol::IProtocol &protocol) { return IEnumTypes::I_LIST == readTypeAux(protocol); }

    inline void operator()(protocol::IProtocol &protocol, std::forward_list<_Tp, _Alloc> &obj) {
        auto size = readSizeAux(protocol);
        auto reader = IReaderType<_Tp>();
        checkTypeAux<_Tp>(reader.readType(protocol));
        auto it = obj.before_begin();
        for (decltype(size) i = 0; i < size; i++) { it = obj.insert_after(it, std::move(reader(protocol))); }
    }

    inline std::forward_list<_Tp, _Alloc> operator()(protocol::IProtocol &protocol) {
        std::forward_list<_Tp, _Alloc> obj;
        (*this)(protocol, obj);
        return std::move(obj);
    }
};

template<typename _Key, typename _Compare, typename _Alloc>
struct ignis::executor::core::io::IReaderType<std::set<_Key, _Compare, _Alloc>> {
    inline bool readType(protocol::IProtocol &protocol) { return IEnumTypes::I_SET == readTypeAux(protocol); }

    inline void operator()(protocol::IProtocol &protocol, std::set<_Key, _Compare, _Alloc> &obj) {
        auto size = readSizeAux(protocol);
        auto reader = IReaderType<_Key>();
        checkTypeAux<_Key>(reader.readType(protocol));
        for (decltype(size) i = 0; i < size; i++) { obj.insert(std::move(reader(protocol))); }
    }

    inline std::set<_Key, _Compare, _Alloc> operator()(protocol::IProtocol &protocol) {
        std::set<_Key, _Compare, _Alloc> obj;
        (*this)(protocol, obj);
        return std::move(obj);
    }
};

template<typename _Value, typename _Hash, typename _Pred, typename _Alloc>
struct ignis::executor::core::io::IReaderType<std::unordered_set<_Value, _Hash, _Pred, _Alloc>> {
    inline bool readType(protocol::IProtocol &protocol) { return IEnumTypes::I_SET == readTypeAux(protocol); }

    inline void operator()(protocol::IProtocol &protocol, std::unordered_set<_Value, _Hash, _Pred, _Alloc> &obj) {
        auto size = readSizeAux(protocol);
        auto reader = IReaderType<_Value>();
        checkTypeAux<_Value>(reader.readType(protocol));
        obj.reserve(size);
        for (decltype(size) i = 0; i < size; i++) { obj.insert(std::move(reader(protocol))); }
    }

    inline std::unordered_set<_Value, _Hash, _Pred, _Alloc> operator()(protocol::IProtocol &protocol) {
        std::unordered_set<_Value, _Hash, _Pred, _Alloc> obj;
        (*this)(protocol, obj);
        return std::move(obj);
    }
};

template<typename _Key, typename _Tp, typename _Compare, typename _Alloc>
struct ignis::executor::core::io::IReaderType<std::map<_Key, _Tp, _Compare, _Alloc>> {
    inline bool readType(protocol::IProtocol &protocol) { return IEnumTypes::I_MAP == readTypeAux(protocol); }

    inline void operator()(protocol::IProtocol &protocol, std::map<_Key, _Tp, _Compare, _Alloc> &obj) {
        auto size = readSizeAux(protocol);
        auto r_key = IReaderType<_Key>();
        auto r_value = IReaderType<_Tp>();
        checkTypeAux<_Key>(r_key.readType(protocol));
        checkTypeAux<_Tp>(r_value.readType(protocol));
        for (decltype(size) i = 0; i < size; i++) {
            auto key = IReaderType<_Key>()(protocol);
            obj[std::move(key)] = std::move(IReaderType<_Tp>()(protocol));
        }
    }

    inline std::map<_Key, _Tp, _Compare, _Alloc> operator()(protocol::IProtocol &protocol) {
        std::map<_Key, _Tp, _Compare, _Alloc> obj;
        (*this)(protocol, obj);
        return std::move(obj);
    }
};

template<typename _Key, typename _Tp, typename _Hash, typename _Pred, typename _Alloc>
struct ignis::executor::core::io::IReaderType<std::unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc>> {
    inline bool readType(protocol::IProtocol &protocol) { return IEnumTypes::I_MAP == readTypeAux(protocol); }

    inline void operator()(protocol::IProtocol &protocol, std::unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc> &obj) {
        auto size = readSizeAux(protocol);
        auto r_key = IReaderType<_Key>();
        auto r_value = IReaderType<_Tp>();
        checkTypeAux<_Key>(r_key.readType(protocol));
        checkTypeAux<_Tp>(r_value.readType(protocol));
        obj.reserve(size);
        for (decltype(size) i = 0; i < size; i++) {
            auto key = IReaderType<_Key>()(protocol);
            obj[std::move(key)] = std::move(IReaderType<_Tp>()(protocol));
        }
    }

    inline std::unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc> operator()(protocol::IProtocol &protocol) {
        std::unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc> obj;
        (*this)(protocol, obj);
        return std::move(obj);
    }
};

template<typename _T1, typename _T2>
struct ignis::executor::core::io::IReaderType<std::pair<_T1, _T2>> {
    inline bool readType(protocol::IProtocol &protocol) {
        return IEnumTypes::I_PAIR == readTypeAux(protocol) && first_reader.readType(protocol) &&
               second_reader.readType(protocol);
    }

    inline void operator()(protocol::IProtocol &protocol, std::pair<_T1, _T2> &obj) {
        first_reader(protocol, obj.first);
        second_reader(protocol, obj.second);
    }

    inline std::pair<_T1, _T2> operator()(protocol::IProtocol &protocol) {
        std::pair<_T1, _T2> obj;
        (*this)(protocol, obj);
        return std::move(obj);
    }

private:
    IReaderType<_T1> first_reader;
    IReaderType<_T2> second_reader;
};

template<typename _Tp>
struct ignis::executor::core::io::IReaderType<_Tp *> {
    inline bool readType(protocol::IProtocol &protocol) { return reader.readType(protocol); }

    inline void operator()(protocol::IProtocol &protocol, _Tp *&obj) { reader(protocol, *obj); }

    inline _Tp *operator()(protocol::IProtocol &protocol) {
        _Tp *obj = new _Tp();
        (*this)(protocol, obj);
        return obj;
    }

private:
    IReaderType<_Tp> reader;
};

template<typename _Tp>
struct ignis::executor::core::io::IReaderType<std::shared_ptr<_Tp>> {
    inline bool readType(protocol::IProtocol &protocol) { return reader.readType(protocol); }

    inline void operator()(protocol::IProtocol &protocol, std::shared_ptr<_Tp> &obj) { reader(protocol, *obj); }

    inline std::shared_ptr<_Tp> operator()(protocol::IProtocol &protocol) {
        auto obj = std::make_shared<_Tp>();
        (*this)(protocol, obj);
        return obj;
    }

private:
    IReaderType<_Tp> reader;
};