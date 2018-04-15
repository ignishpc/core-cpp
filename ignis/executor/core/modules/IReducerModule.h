
#ifndef IGNIS_IREDUCERMODULE_H
#define IGNIS_IREDUCERMODULE_H

#include "IModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IReducerModule : public IgnisModule{
                public:
                    IReducerModule(std::shared_ptr<IExecutorData> &executor_data);
                };
            }
        }
    }
}


#endif
