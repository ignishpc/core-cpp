
#ifndef IGNIS_ISOURCE_H
#define IGNIS_ISOURCE_H

#include "ignis/rpc/ISource_types.h"
#include "ignis/executor/core/io/IWriter.h"
#include "ignis/executor/core/protocol/IObjectProtocol.h"
#include "ignis/executor/core/transport/IMemoryBuffer.h"

namespace ignis {
    namespace driver {
        namespace api {
            class ISource {
            public:

                ISource(const std::string &src) {
                    rpc::IEncoded e;
                    e.__set_name(src);
                    inner.__set_obj(e);
                }

                template<typename Tp>
                ISource &addParam(const std::string &name, const Tp &value) {
                    auto buffer = std::make_shared<executor::core::transport::IMemoryBuffer>();
                    executor::core::protocol::IObjectProtocol proto(buffer);
                    executor::core::io::IWriter<Tp>()(proto, value);
                    inner.params[name] = buffer->getBufferAsString();
                }

                const rpc::ISource &rpc() const {
                    return inner;
                }

            private:
                rpc::ISource inner;
            };

        }
    }
}

#endif
