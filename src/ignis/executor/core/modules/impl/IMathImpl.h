
#ifndef IGNIS_IMATHIMPL_H
#define IGNIS_IMATHIMPL_H

#include "IBaseImpl.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                namespace impl {
                    class IMathImpl : public IBaseImpl {
                    public:
                        IMathImpl(std::shared_ptr<IExecutorData> &executorData);

                        template<typename Tp>
                        void sample(const bool withReplacement, const std::vector<int64_t> & num, const int32_t seed);

                        int64_t count();

                        template<typename Tp>
                        int64_t sampleByKeyFilter();

                        template<typename Tp>
                        void sampleByKey(const bool withReplacement, const int32_t seed);

                        template<typename Tp>
                        void countByKey();

                        template<typename Tp>
                        void countByValue();

                    private:
                        template<typename Tp>
                        void countByReduce(std::unordered_map<Tp, int64_t> &acum);
                    };
                }// namespace impl
            }    // namespace modules
        }        // namespace core
    }            // namespace executor
}// namespace ignis

#include "IMathImpl.tcc"

#endif
