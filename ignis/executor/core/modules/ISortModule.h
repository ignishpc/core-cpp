
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

                    virtual void localSort(const bool ascending) override;

                    virtual void localCustomSort(const rpc::ISource &sf,const bool ascending) override;

                    virtual void sampling(const int64_t sampleSize, const int64_t idx, const std::string& master) override;

                    virtual void getPivots() override;

                    virtual void findPivots(const std::vector<std::string> & nodes) override;

                    virtual void exchangePartitions(const int64_t idx, const std::vector<std::string> & nodes) override;

                    virtual void mergePartitions() override;

                    virtual ~ISortModule();

                private:

                    void mergeSort(api::function::IFunction2<storage::IObject::Any, storage::IObject::Any, bool> &less,
                                   const bool ascending);
                };
            }
        }
    }
}


#endif
