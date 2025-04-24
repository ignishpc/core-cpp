
#ifndef IGNIS_ICLIENTPOOL_H
#define IGNIS_ICLIENTPOOL_H

#include "IClient.h"
#include <list>
#include <mutex>

namespace ignis {
    namespace driver {
        namespace core {
            class IClientPool {
            public:
                IClientPool(const std::string& usock, int compression);

                std::shared_ptr<IClient> getClient();

            private:
                std::string usock;
                int compression;
                std::mutex mutex;
                std::list<std::shared_ptr<IClient>> clients;
                std::list<std::shared_ptr<IClient>> queue;
            };
        }// namespace core
    }    // namespace driver
}// namespace ignis

#endif
