
#ifndef IGNIS_IPIPEIMPL_H
#define IGNIS_IPIPEIMPL_H

#include "IBaseImpl.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                namespace impl {
                    class IPipeImpl : public IBaseImpl {
                    public:
                        IPipeImpl(std::shared_ptr<IExecutorData> &executorData);

                        template <typename Function>
                        void map();

                        template <typename Function>
                        void filter();

                        template <typename Function>
                        void flatmap();

                        template <typename Function>
                        void mapPartitions(bool preservesPartitioning);

                        template <typename Function>
                        void mapPartitionsWithIndex(bool preservesPartitioning);

                        template <typename Function>
                        void applyPartition();//TODO

                        virtual ~IPipeImpl();
                    };
                }
            }
        }
    }
}


#include "IPipeImpl.tcc"
#endif
