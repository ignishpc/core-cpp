
#ifndef IGNIS_IREDUCEIMPL_H
#define IGNIS_IREDUCEIMPL_H

#include "IBaseImpl.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                namespace impl {
                    class IReduceImpl : public IBaseImpl {
                    public:
                        IReduceImpl(std::shared_ptr<IExecutorData> &executorData);

                        template<typename Function>
                        void reduce();

                        template<typename Function>
                        void treeReduce(int64_t depth);

                        virtual ~IReduceImpl();

                    private:
                        template<typename Function, typename Tp>
                        inline Tp reducePartition(Function& f, storage::IPartition<Tp> &part);
                    };
                }
            }
        }
    }
}

#include "IReduceImpl.tcc"

#endif

