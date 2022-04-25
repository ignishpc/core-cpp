
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

                        template<typename Tp>
                        int64_t partitionApproxSize();

                        void plainFile(const std::string &path, int64_t minPartitions, char delim);

                        void textFile(const std::string &path, int64_t minPartitions);

                        void partitionObjectFileVoid(const std::string &path, int64_t first, int64_t partitions);

                        void partitionJsonFileVoid(const std::string &path, int64_t first, int64_t partitions);

                        template<typename Tp>
                        void partitionObjectFile(const std::string &path, int64_t first, int64_t partitions);

                        void partitionTextFile(const std::string &path, int64_t first, int64_t partitions);

                        template<typename Tp>
                        void partitionJsonFile(const std::string &path, int64_t first, int64_t partitions);

                        template<typename Tp>
                        void saveAsObjectFile(const std::string &path, int8_t compression, int64_t first);

                        template<typename Tp>
                        void saveAsTextFile(const std::string &path, int64_t first);

                        template<typename Tp>
                        void saveAsJsonFile(const std::string &path, int64_t first, bool pretty);

                        std::string partitionFileName(const std::string &path, int64_t index);

                        std::ifstream openFileRead(const std::string &path);

                        std::ofstream openFileWrite(const std::string &path);

                        virtual ~IIOImpl();
                    private:
                        int ioCores();
                    };
                }// namespace impl
            }    // namespace modules
        }        // namespace core
    }            // namespace executor
}// namespace ignis

#include "IIOImpl.tcc"

#endif
