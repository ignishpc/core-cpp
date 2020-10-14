
#ifndef IGNIS_IVOIDPARTITION_H
#define IGNIS_IVOIDPARTITION_H

#include "IPartition.h"
#include "ignis/executor/core/transport/IMemoryBuffer.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {


                class IVoidPartition : public IPartition<char> {
                public:
                    static const std::string TYPE;
                    typedef char VOID_TYPE;

                    IVoidPartition(size_t size = 1024);

                    IVoidPartition(const std::string& path);

                    virtual std::shared_ptr<api::IReadIterator<VOID_TYPE>> readIterator();

                    virtual std::shared_ptr<api::IWriteIterator<VOID_TYPE>> writeIterator();

                    virtual void read(std::shared_ptr<transport::ITransport> &trans);

                    virtual void write(std::shared_ptr<transport::ITransport> &trans, int8_t compression);

                    virtual void copyFrom(IPartition<VOID_TYPE> &source);

                    virtual void moveFrom(IPartition<VOID_TYPE> &source);

                    virtual std::shared_ptr<IPartition<VOID_TYPE>> clone();

                    virtual size_t size();

                    virtual size_t bytes();

                    virtual void clear();

                    virtual void fit();

                    std::shared_ptr<transport::ITransport> readTransport();

                    virtual const std::string &type();

                    virtual ~IVoidPartition();

                private:
                    std::shared_ptr<core::transport::IMemoryBuffer> buffer;
                    std::shared_ptr<transport::IFileTransport> file;
                    std::string path;
                    int copies;
                };
            }// namespace storage
        }    // namespace core
    }        // namespace executor
}// namespace ignis

#endif
