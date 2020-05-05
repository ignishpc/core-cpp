#include "INativeWriter.h"


template<typename Tp>
struct ignis::executor::core::io::isContiguous {
    inline bool operator()() {
        return std::is_integral<Tp>::value || std::is_floating_point<Tp>::value;
    }
};

template<typename Tp1, typename Tp2>
struct ignis::executor::core::io::isContiguous<std::pair<Tp1, Tp2>> {
    inline bool operator()() {
        return isContiguous<Tp1>()() && isContiguous<Tp2>()();
    }
};

template<typename Tp>
struct ignis::executor::core::io::INativeWriterType {

    inline void operator()(protocol::IProtocol &protocol, const bool &obj) {
        if (ignis::executor::core::io::isContiguous<Tp>()()) {
            protocol.writeBool(true);
            protocol.getTransport()->write((uint8_t * ) & obj, sizeof(Tp));
        } else {
            protocol.writeBool(false);
            IWriterType<Tp>()(protocol, obj);
        }
    }
};

template<typename _Tp, typename _Alloc>
struct ignis::executor::core::io::INativeWriterType<std::vector<_Tp,_Alloc>> {

    inline void operator()(protocol::IProtocol &protocol, const std::vector<_Tp,_Alloc> &obj) {
        if (ignis::executor::core::io::isContiguous<_Tp>()()) {
            protocol.writeBool(true);
            protocol.writeI64(obj.size());
            protocol.getTransport()->write((uint8_t * ) & obj[0], (uint32_t) obj.size() * sizeof(_Tp));
        } else {
            protocol.writeBool(false);
            IWriterType<std::vector<_Tp,_Alloc>>()(protocol, obj);
        }
    }
};

