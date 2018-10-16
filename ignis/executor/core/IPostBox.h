
#ifndef IGNIS_IPOSTBOX_H
#define IGNIS_IPOSTBOX_H

#include <unordered_map>
#include <mutex>
#include "IMessage.h"


namespace ignis {
    namespace executor {
        namespace core {

            class IPostBox {
            public:

                IPostBox();

                std::unordered_map<size_t, IMessage> getInBox();

                std::unordered_map<size_t, IMessage> popInBox();

                void newInMessage(size_t id, const IMessage &msg);

                void clearInBox();

                std::unordered_map<size_t, IMessage> getOutBox();

                std::unordered_map<size_t, IMessage> popOutBox();

                void newOutMessage(size_t id, const IMessage &msg);

                void clearOutBox();

                virtual ~IPostBox();

            private:
                std::unordered_map<size_t, IMessage> outbox;
                std::unordered_map<size_t, IMessage> inbox;

            };
        }
    }
}

#endif
