

#ifndef IGNIS_ICALLBACK_H
#define IGNIS_ICALLBACK_H

#include "IDriverContext.h"
#include "ignis/rpc/executor/IExecutorServerModule.h"

namespace ignis {
    namespace driver {
        namespace core {
            class ICallBack {
            public:
                ICallBack(int port, int compression);

                core::IDriverContext &getDriverContext();

                virtual ~ICallBack();

            private:
                std::shared_ptr<core::IDriverContext> driverContext;

                std::shared_ptr<rpc::executor::IExecutorServerModuleIf> server;
            };
        }// namespace core
    }    // namespace driver
}// namespace ignis

#endif
