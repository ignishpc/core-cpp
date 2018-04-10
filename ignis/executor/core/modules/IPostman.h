
#ifndef IGNIS_IPOSTMAN_H
#define IGNIS_IPOSTMAN_H

#include "IModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IPostman : public IgnisModule {
                public:
                    IPostman(std::shared_ptr<IExecutorData> &executor_data);
                };
            }
        }
    }
}

#endif
