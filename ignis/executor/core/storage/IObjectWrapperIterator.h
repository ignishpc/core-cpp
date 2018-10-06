
#ifndef IGNIS_IOBJECTWRAPPERITERATOR_H
#define IGNIS_IOBJECTWRAPPERITERATOR_H

#include "IObject.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {
                class IObjectWrapperIterator : public IObject{
                public:
                    IObjectWrapperIterator(const std::shared_ptr<iterator::ICoreReadIterator<IObject::Any>> &it, size_t size);

                    std::shared_ptr<iterator::ICoreReadIterator<Any>> readIterator() override;

                    std::shared_ptr<iterator::ICoreWriteIterator<Any>> writeIterator() override;

                    void read(std::shared_ptr<transport::TTransport> trans) override;

                    void write(std::shared_ptr<transport::TTransport> trans, int8_t compression) override;

                    virtual void copyFrom(IObject& source) override;

                    virtual void copyTo(IObject& source) override;

                    virtual void moveFrom(IObject& source) override;

                    virtual void moveTo(IObject& source) override;

                    size_t getSize() override;

                    void setSize(size_t size);

                    virtual std::string getType() override;

                    virtual ~IObjectWrapperIterator();

                private:
                    std::shared_ptr<iterator::ICoreReadIterator<IObject::Any>> it;
                    size_t size;
                };
            }
        }
    }
}

#endif
