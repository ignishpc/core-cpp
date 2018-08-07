
#ifndef IGNIS_IPOSTBOX_H
#define IGNIS_IPOSTBOX_H

#include <vector>
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

                    std::vector<IMessage> getMessages();

                    void newMessage(IMessage &msg);

                    virtual ~IPostBox();

                private:
                    friend modules::IPostmanModule;

                    std::vector<IMessage> getOutMessages();

                    void newInMessage(IMessage &msg);

                    std::mutex mutex;
                    std::vector<IMessage> outbox;
                    std::vector<IMessage> inbox;

                };
            }
        }
    }
}

#endif
