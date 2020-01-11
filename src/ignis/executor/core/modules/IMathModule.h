
#ifndef IGNIS_IMATHMODULE_H
#define IGNIS_IMATHMODULE_H

#include "IModule.h"
#include "ignis/rpc/executor/IMathModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IMathModule : public IModule, public rpc::executor::IMathModuleIf {
                public:
                    IMathModule(std::shared_ptr<IExecutorData> &executorData);

                    void sample(const bool withReplacement, const double fraction, const int32_t seed);

                    void takeSample(const bool withReplacement, const double fraction, const int32_t seed);

                    int64_t count();

                    void max(const rpc::ISource &cmp);

                    void min(const rpc::ISource &cmp);

                    virtual ~IMathModule();

                private:
                };
            }
        }
    }
}

#endif
