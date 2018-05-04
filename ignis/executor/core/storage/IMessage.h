
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

                    IMessage(const std::string &host, int port, const std::string shared_memory,
                             const std::shared_ptr<IObject> &obj) : host(host),
                                                                    port(port),
                                                                    shared_memory(shared_memory),
                                                                    obj(obj) {}

                    IMessage(const std::shared_ptr<IObject> &obj) : host(""),
                                                                    port(0),
                                                                    obj(obj) {}

                    std::string &getHost() {
                        return host;
                    }

                    int getPort() {
                        return port;
                    }

                    std::shared_ptr<IObject> &getObj() {
                        return obj;
                    }

                    std::string &getSharedMemory() {
                        return shared_memory;
                    }

                    virtual ~IMessage() {}

                private:
                    std::string host;
                    int port;
                    std::string shared_memory;
                    std::shared_ptr<IObject> obj;
                };
            }
        }
    }
}

#endif
