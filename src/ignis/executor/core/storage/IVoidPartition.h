
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

                    virtual std::shared_ptr<api::IReadIterator<VOID_TYPE>> readIterator();

                    virtual std::shared_ptr<api::IWriteIterator<VOID_TYPE>> writeIterator();

                    virtual void read(std::shared_ptr<transport::ITransport> &trans);

                    virtual void write(std::shared_ptr<transport::ITransport> &trans, int8_t compression);

                    template<typename Tp>
                    void write(IPartition<Tp> &partition) {
                        partition.read(reinterpret_cast<std::shared_ptr<core::transport::ITransport>&>(buffer));
                    }

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
                };
            }
        }
    }
}

#endif
