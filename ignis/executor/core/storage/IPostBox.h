
#ifndef IGNIS_IPOSTBOX_H
#define IGNIS_IPOSTBOX_H

#include <unordered_map>
#include <mutex>
#include "IMessage.h"


namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IPostmanModule;
            }

            namespace storage {
                class IPostBox {
                public:

                    IPostBox();

                    std::unordered_map<size_t, IMessage> getMessages();

                    void newMessage(size_t id, IMessage &msg);

                    virtual ~IPostBox();

                private:
                    friend modules::IPostmanModule;

                    std::unordered_map<size_t, IMessage> getOutMessages();

                    void newInMessage(size_t id, IMessage &msg);

                    std::mutex mutex;
                    std::unordered_map<size_t, IMessage> outbox;
                    std::unordered_map<size_t, IMessage> inbox;

                };
            }
        }
    }
}

#endif
