
#ifndef IGNIS_IOBJECTWRAPPER_H
#define IGNIS_IOBJECTWRAPPER_H

#include "IObject.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {
                class IObjectWrapper : public IObject{
                public:

                    IObjectWrapper(const std::shared_ptr<IObject> &to_read, const std::shared_ptr<IObject> &to_write);

                    virtual std::shared_ptr<iterator::ICoreReadIterator<Any>> readIterator() override;

                    virtual std::shared_ptr<iterator::ICoreWriteIterator<Any>> writeIterator() override;

                    virtual void read(std::shared_ptr<transport::TTransport> trans) override;

                    virtual void write(std::shared_ptr<transport::TTransport> trans, int8_t compression) override;

                    virtual void copyFrom(IObject& source) override;

                    virtual void moveFrom(IObject& source) override;

                    virtual size_t getSize() override;

                    virtual bool hasSize() override;

                    virtual void clear() override;

                    virtual void fit() override;

                    virtual std::string getType() override;

                    virtual std::shared_ptr<api::IManager<Any>> getManager() override;

                    virtual IObject& setManager(std::shared_ptr<api::IManager<Any>> manager) override;

                    virtual ~IObjectWrapper();

                private:
                    std::shared_ptr<IObject> to_read;
                    std::shared_ptr<IObject> to_write;
                };
            }
        }
    }
}

#endif
