
#ifndef IGNIS_IDRIVERMODULE_H
#define IGNIS_IDRIVERMODULE_H

#include "IModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IDriverModule : public IgnisModule{
                public:
                    IDriverModule(std::shared_ptr<IExecutorData> &executor_data);

                    virtual ~IDriverModule();
                };
            }
        }
    }
}

#endif
