
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

                    IMessage(const std::string &addr, const std::shared_ptr<IObject> &obj) : addr(addr), obj(obj) {}

                    std::string &getAddr() {
                        return addr;
                    }

                    std::shared_ptr<IObject> &getObj() {
                        return obj;
                    }

                    virtual ~IMessage() {}

                private:
                    std::string addr;
                    std::shared_ptr<IObject> obj;
                };
            }
        }
    }
}

#endif
