
#ifndef IGNIS_IMAPPERMODULE_H
#define IGNIS_IMAPPERMODULE_H

#include "IModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IMapperModule : public IgnisModule{
                public:
                    IMapperModule(std::shared_ptr<IExecutorData> &executor_data);
                };
            }
        }
    }
}

#endif
