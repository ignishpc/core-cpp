
#ifndef IGNIS_IDYNAMICTYPES_H
#define IGNIS_IDYNAMICTYPES_H

#include "IPropertyParser.h"
#include "ignis/executor/core/transport/ITransport.h"
#include <ignis/executor/core/protocol/IProtocol.h>
#include <string>

namespace ignis {
    namespace executor {
        namespace core {
            class IDynamicTypes {
            public:
                IDynamicTypes(IPropertyParser& properties);

                std::string typeFromBytes(std::shared_ptr<transport::ITransport> &trans, bool inColl = true);

                std::string compileType(const std::string &type);

            private:
                IPropertyParser& properties;

                static int8_t checkRange(int8_t tp);

                static void (*knownTypes[])(protocol::IProtocol &protocol, std::string &name);
            };
        }// namespace core
    }    // namespace executor
}// namespace ignis
#endif
