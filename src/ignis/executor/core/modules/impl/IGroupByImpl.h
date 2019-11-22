
#ifndef IGNIS_IGROUPBYIMPL_H
#define IGNIS_IGROUPBYIMPL_H

#include "IBaseImpl.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                namespace impl {
                    class IGroupByImpl : public IBaseImpl {
                    public:
                        IGroupByImpl(std::shared_ptr<IExecutorData> &executorData);

                        virtual ~IGroupByImpl();

                        template<typename Function>
                        void groupBy();

                        template<typename Function>
                        void groupBy(int64_t numPartitions);

                    };
                }
            }
        }
    }
}

#include "IGroupByImpl.tcc"
#endif

