
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

                        template<typename Function>
                        void map();

                        template<typename Function>
                        void filter();

                        template<typename Function>
                        void flatmap();

                        template<typename Function>
                        void keyBy();

                        template<typename Function>
                        void mapPartitions(bool preservesPartitioning);

                        template<typename Function>
                        void mapPartitionsWithIndex(bool preservesPartitioning);

                        template<typename Function, typename Tp>
                        void mapExecutor();

                        template<typename Function, typename Tp, typename R>
                        void mapExecutorTo();

                        template<typename Function>
                        void foreach ();

                        template<typename Function>
                        void foreachPartition();

                        template<typename Tp>
                        void take(int64_t num);

                        template<typename Tp>
                        void keys();

                        template<typename Tp>
                        void values();

                        template<typename Tp, typename Function>
                        void flatMapValues();

                        template<typename Tp, typename Function>
                        void mapValues();

                        virtual ~IPipeImpl();
                    };
                }// namespace impl
            }    // namespace modules
        }        // namespace core
    }            // namespace executor
}// namespace ignis


#include "IPipeImpl.tcc"

#endif
