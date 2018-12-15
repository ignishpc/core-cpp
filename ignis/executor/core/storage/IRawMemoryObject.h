
#ifndef IGNIS_IRAWMEMORYOBJECT_H
#define IGNIS_IRAWMEMORYOBJECT_H

#include <memory>
#include "../../../data/IMemoryBuffer.h"
#include "IRawObject.h"


namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {
                class IRawMemoryObject : public IRawObject {
                public:

                    static const std::string TYPE;

                    IRawMemoryObject(int8_t compression, uint32_t sz = 100 * 1024 * 1024);

                    virtual std::shared_ptr<iterator::ICoreReadIterator<Any>> readIterator() override;

                    virtual std::shared_ptr<iterator::ICoreWriteIterator<Any>> writeIterator() override;

                    virtual void read(std::shared_ptr<transport::TTransport> trans) override;

                    virtual void write(std::shared_ptr<transport::TTransport> trans, int8_t compression) override;

                    virtual void clear() override;

                    virtual void copyTo(IObject &target) override;

                    virtual void fit() override;

                    virtual std::string getType() override;

                    virtual ~IRawMemoryObject();

                private:

                    IRawMemoryObject(const std::shared_ptr<data::IMemoryBuffer> &buffer, int8_t compression, size_t elems,
                                     int8_t type, bool read_only);

                    IRawMemoryObject(const std::shared_ptr<data::IMemoryBuffer> &buffer, int8_t compression);

                    inline std::shared_ptr<data::IMemoryBuffer> readObservation();

                    virtual void flush();

                    std::shared_ptr<data::IMemoryBuffer> raw_memory;
                    bool read_only;
                };
            }
        }
    }
}

#endif
