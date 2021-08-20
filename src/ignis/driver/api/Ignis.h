
#ifndef IGNIS_IGNIS_H
#define IGNIS_IGNIS_H

#include "ignis/driver/core/IClientPool.h"
#include "ignis/driver/core/IDriverContext.h"

namespace ignis {
    namespace driver {
        namespace api {
            class IProperties;

            class ICluster;

            class IWorker;

            class IAbstractDataFrame;

            class Ignis {
            public:
                static void start();

                static void stop();

            private:
                friend IProperties;
                friend ICluster;
                friend IWorker;
                friend IAbstractDataFrame;

                Ignis() {}

                static core::IDriverContext &driverContext();

                static core::IClientPool &clientPool();
            };
        }// namespace api
    }    // namespace driver
}// namespace ignis

#endif
