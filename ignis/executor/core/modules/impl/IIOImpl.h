
#ifndef IGNIS_IIOIMPL_H
#define IGNIS_IIOIMPL_H

#include "IBaseImpl.h"
#include <fstream>

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                namespace impl {
                    class IIOImpl : public IBaseImpl {
                    public:
                        IIOImpl(std::shared_ptr<IExecutorData> &executorData);

                        void textFile(const std::string &path, int64_t minPartitions);

                        void openPartitionObjectFileUnknown(const std::string &path, int64_t first, int64_t partitions);

                        template<typename Tp>
                        void openPartitionObjectFile(const std::string &path, int64_t first, int64_t partitions);

                        template<typename Tp>
                        void saveAsPartitionObjectFile(const std::string &path, int8_t compression, int64_t first);

                        template<typename Tp>
                        void saveAsTextFile(const std::string &path, int64_t first);

                        template<typename Tp>
                        void saveAsJsonFile(const std::string &path, int64_t first);

                        std::string partitionFileName(const std::string &path, int64_t index);

                        std::ifstream openFileRead(const std::string &path);

                        std::ofstream openFileWrite(const std::string &path);

                        virtual ~IIOImpl();
                    };
                }
            }
        }
    }
}

#include "IIOImpl.tcc"

#endif

