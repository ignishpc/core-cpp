
#ifndef IGNIS_IBASEIMPL_H
#define IGNIS_IBASEIMPL_H

#include <memory>
#include "ignis/executor/core/IMpi.h"
#include "ignis/executor/core/exception/IInvalidArgument.h"
#include "ignis/executor/core/exception/ILogicError.h"
#include "ignis/executor/core/storage/IMemoryPartition.h"
#include "ignis/executor/core/storage/IRawMemoryPartition.h"
#include "ignis/executor/core/storage/IDiskPartition.h"
#include "ignis/executor/core/IExecutorData.h"
#include "ignis/executor/core/ILog.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                namespace impl {
                    class IBaseImpl {
                    public:
                        IBaseImpl(std::shared_ptr<IExecutorData> &executor_data);

                        void registerType(const std::shared_ptr<selector::ITypeSelector> &type);

                        virtual ~IBaseImpl();

                    protected:
                        std::shared_ptr<IExecutorData> executor_data;
                    };
                }
            }
        }
    }
}

#endif

