
#ifndef IGNIS_IIOMODULE_H
#define IGNIS_IIOMODULE_H

#include "IModule.h"
#include "impl/IIOImpl.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IIOModule : public IModule {

                    IIOModule(std::shared_ptr<IExecutorData> &executor_data);

                    virtual ~IIOModule();

                };
            }
        }
    }
}
#endif
