
#ifndef EXECUTORCPP_IGNISMODULE_H
#define EXECUTORCPP_IGNISMODULE_H

#include <memory>
#include "../IExecutorData.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IgnisModule {

                public:
                    IgnisModule(std::shared_ptr<IExecutorData> &executor_data);

                    virtual ~IgnisModule();

                protected:
                    std::shared_ptr<IExecutorData> executor_data;

                };
            }
        }
    }
}


#endif
