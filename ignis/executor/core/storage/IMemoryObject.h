
#ifndef IGNIS_IMEMORYOBJECT_H
#define IGNIS_IMEMORYOBJECT_H

#include "IObject.h"
#include "../../../data/IManager.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {
                class IMemoryObject : public IObject {

                public:

                    IMemoryObject(const std::shared_ptr<data::IManager<Any>> &manager, size_t size  = 1000);

                    std::shared_ptr<ICoreReadIterator<Any>> readIterator() override;

                    std::shared_ptr<ICoreWriteIterator<Any>> writeIterator() override;

                    void read(std::shared_ptr<apache::thrift::transport::TTransport> trans) override;

                    void clear() override;

                    void
                    write(std::shared_ptr<apache::thrift::transport::TTransport> trans, int8_t compression) override;

                    size_t getSize() override;

                    virtual ~IMemoryObject();

                private:
                    std::vector<IObject::Any> *data;
                    std::shared_ptr<data::IManager<Any>> manager;
                    std::shared_ptr<IObject::Handle<std::vector<IObject::Any>>> handle;

                };
            }
        }
    }
}

#endif
