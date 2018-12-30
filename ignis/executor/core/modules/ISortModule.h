
#ifndef IGNIS_ISORTMODULE_H
#define IGNIS_ISORTMODULE_H

#include "IModule.h"
#include "../../../rpc/executor/ISortModule.h"
#include "../../api/function/IFunction2.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class ISortModule : public IgnisModule, public ignis::rpc::executor::ISortModuleIf {
                public:
                    ISortModule(std::shared_ptr<IExecutorData> &executor_data);

                    virtual void localSort(bool ascending) override;

                    virtual void localCustomSort(const rpc::ISource &sf, bool ascending) override;

                    virtual ~ISortModule();

                private:

                    void mergeSort(api::function::IFunction2<storage::IObject::Any, storage::IObject::Any, bool>& less,
                                   bool ascending);
                };
            }
        }
    }
}


#endif
