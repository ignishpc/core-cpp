
#ifndef IGNIS_IOBJECTWRAPPERITERATOR_H
#define IGNIS_IOBJECTWRAPPERITERATOR_H

#include "IObject.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {
                class IObjectWrapperIterator : public IObject{
                public:
                    IObjectWrapperIterator(const std::shared_ptr<ICoreReadIterator<IObject::Any>> &it, size_t size);

                    std::shared_ptr<ICoreReadIterator<Any>> readIterator() override;

                    std::shared_ptr<ICoreWriteIterator<Any>> writeIterator() override;

                    void read(std::shared_ptr<apache::thrift::transport::TTransport> trans) override;

                    void
                    write(std::shared_ptr<apache::thrift::transport::TTransport> trans, int8_t compression) override;

                    size_t getSize() override;

                    void setSize(size_t size);

                    virtual ~IObjectWrapperIterator();

                private:
                    std::shared_ptr<ICoreReadIterator<IObject::Any>> it;
                    size_t size;
                };
            }
        }
    }
}

#endif
