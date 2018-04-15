
#ifndef IGNIS_ISORTMODULE_H
#define IGNIS_ISORTMODULE_H

#include "IModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class ISortModule : public IgnisModule{
                public:
                    ISortModule(std::shared_ptr<IExecutorData> &executor_data);
                };
            }
        }
    }
}


#endif
