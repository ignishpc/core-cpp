
#ifndef IGNIS_IMESSAGE_H
#define IGNIS_IMESSAGE_H

#include <string>
#include <memory>
#include "storage/IObject.h"

namespace ignis {
    namespace executor {
        namespace core {

            class IMessage {
            public:

                IMessage() {}

                IMessage(const std::string &addr, const std::shared_ptr<storage::IObject> &obj) : addr(addr), obj(obj) {}

                std::string &getAddr() {
                    return addr;
                }

                std::shared_ptr<storage::IObject> &getObj() {
                    return obj;
                }

                virtual ~IMessage() {}

            private:
                std::string addr;
                std::shared_ptr<storage::IObject> obj;
            };
        }
    }
}

#endif
