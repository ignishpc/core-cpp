
#ifndef IGNIS_IMESSAGE_H
#define IGNIS_IMESSAGE_H

#include <string>
#include <memory>
#include "IObject.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {
                class IMessage {
                public:

                    IMessage() {}

                    IMessage(const std::string &host, int port, const std::shared_ptr<IObject> &obj) : host(host),
                                                                                                       port(port),
                                                                                                       obj(obj) {}

                private:
                    std::string host;
                    int port;
                    std::shared_ptr<IObject> obj;
                };
            }
        }
    }
}

#endif
