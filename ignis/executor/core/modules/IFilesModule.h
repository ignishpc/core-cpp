
#ifndef IGNIS_IFILESMODULE_H
#define IGNIS_IFILESMODULE_H

#include "IModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IFilesModule : public IgnisModule{
                public:
                    IFilesModule(std::shared_ptr<IExecutorData> &executor_data);
                };
            }
        }
    }
}

#endif
