
#ifndef IGNIS_INATIVEWRITER_H
#define IGNIS_INATIVEWRITER_H

#include "IWriter.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace io {

                template<typename T>
                inline void writeNativeType(protocol::IProtocol &protocol) {
                    protocol.writeBinary(RTTInfo::from<T>().getStandardName());
                }

                template<typename Tp>
                struct isContiguous;

                template<typename T>
                struct INativeWriterType;

                template<typename T>
                class INativeWriter {
                public:
                    virtual void operator()(protocol::IProtocol &protocol, const T &obj) { writer(protocol, obj); }

                private:
                    INativeWriterType<T> writer;
                };

            }// namespace io
        }    // namespace core
    }        // namespace executor
}// namespace ignis

#include "INativeWriter.tcc"

#endif
