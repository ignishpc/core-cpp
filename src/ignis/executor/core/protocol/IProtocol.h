
#ifndef IGNIS_IPROTOCOL_H
#define IGNIS_IPROTOCOL_H

#include <thrift/protocol/TProtocol.h>

namespace ignis {
    namespace executor {
        namespace core {
            namespace protocol {
                using IProtocol = apache::thrift::protocol::TProtocol;
            }
        }// namespace core
    }    // namespace executor
}// namespace ignis

#endif
