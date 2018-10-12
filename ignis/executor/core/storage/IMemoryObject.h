
#ifndef IGNIS_IMEMORYOBJECT_H
#define IGNIS_IMEMORYOBJECT_H

#include "IObject.h"
#include "../../api/IManager.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {
                class IMemoryObject : public IObject {

                public:

                    static const std::string TYPE;

                    IMemoryObject(const std::shared_ptr<api::IManager<Any>> &manager, size_t size  = 1000);

                    virtual std::shared_ptr<iterator::ICoreReadIterator<Any>> readIterator() override;

                    virtual std::shared_ptr<iterator::ICoreWriteIterator<Any>> writeIterator() override;

                    virtual void read(std::shared_ptr<transport::TTransport> trans) override;

                    virtual void write(std::shared_ptr<transport::TTransport> trans, int8_t compression) override;

                    virtual void copyFrom(IObject& source) override;

                    virtual void moveFrom(IObject& source) override;

                    virtual size_t getSize() override;

                    virtual void clear() override;

                    virtual void fit() override;

                    virtual std::string getType() override;

                    virtual ~IMemoryObject();

                private:
                    std::vector<IObject::Any> *data;
                    std::shared_ptr<api::ICollectionManager<Any>> collection_manager;
                };
            }
        }
    }
}

#endif
