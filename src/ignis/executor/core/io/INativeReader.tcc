#include "INativeReader.h"

template<typename Tp>
struct ignis::executor::core::io::INativeReaderType {

    inline void operator()(protocol::IProtocol &protocol, Tp &obj) {
        bool contiguous;
        protocol.readBool(contiguous);
        if (contiguous) {
            checkNativeType<Tp>(protocol);
            protocol.getTransport()->read((uint8_t *) &obj, sizeof(Tp));
        } else {
            IReaderType<Tp>()(protocol, obj);
        }
    }

    inline Tp operator()(protocol::IProtocol &protocol) {
        Tp obj;
        (*this)(protocol, obj);
        return obj;
    }
};

template<typename _Tp, typename _Alloc>
struct ignis::executor::core::io::INativeReaderType<std::vector<_Tp, _Alloc>> {

    inline void operator()(protocol::IProtocol &protocol, std::vector<_Tp> &obj) {
        bool contiguous;
        protocol.readBool(contiguous);
        if (contiguous) {
            checkNativeType<std::vector<_Tp>>(protocol);
            int64_t len;
            protocol.readI64(len);
            auto first = obj.size();
            obj.resize(len);
            protocol.getTransport()->read((uint8_t *) &obj[first], (uint32_t) len * sizeof(_Tp));
        } else {
            IReaderType<std::vector<_Tp, _Alloc>>()(protocol, obj);
        }
    }

    inline std::vector<_Tp> operator()(protocol::IProtocol &protocol) {
        std::vector<_Tp> obj;
        (*this)(protocol, obj);
        return obj;
    }
};