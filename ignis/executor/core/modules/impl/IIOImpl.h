
#ifndef IGNIS_IIOIMPL_H
#define IGNIS_IIOIMPL_H

#include "IBaseImpl.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                namespace impl {
                    class IIOImpl : public IBaseImpl {
                        IIOImpl(std::shared_ptr<IExecutorData> &executorData);

                        void textFile(const std::string& path, int64_t minPartitions);

                        template <typename Tp>
                        void saveAsObjectFile(const std::string& path, int8_t compression);

                        template <typename Tp>
                        void saveAsTextFile(const std::string& path);

                        virtual ~IIOImpl();
                    };
                }
            }
        }
    }
}

#include "IIOImpl.tcc"
#endif

