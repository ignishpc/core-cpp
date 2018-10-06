
#ifndef IGNIS_IRAWMEMORYOBJECT_H
#define IGNIS_IRAWMEMORYOBJECT_H

#include <memory>
#include <thrift/transport/TBufferTransports.h>
#include "IRawObject.h"


namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {
                class IRawMemoryObject : public IRawObject {
                public:
                    IRawMemoryObject(int8_t compression, uint32_t sz = 100 * 1024 * 1024);

                    virtual std::shared_ptr<iterator::ICoreReadIterator<Any>> readIterator() override;

                    virtual std::shared_ptr<iterator::ICoreWriteIterator<Any>> writeIterator() override;

                    virtual void read(std::shared_ptr<transport::TTransport> trans) override;

                    virtual void write(std::shared_ptr<transport::TTransport> trans, int8_t compression) override;

                    virtual void clear() override;

                    virtual void fit() override;

                    virtual std::string getType() override;

                    virtual ~IRawMemoryObject();

                private:

                    inline bool fastWrite(std::shared_ptr<transport::TTransport> transport) override;

                    inline std::shared_ptr<transport::TMemoryBuffer> readObservation();

                    std::shared_ptr<transport::TMemoryBuffer> raw_memory;
                    std::vector<uint64_t > index;
                };
            }
        }
    }
}

#endif
