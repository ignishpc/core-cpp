
#ifndef IGNIS_IRAWOBJECT_H
#define IGNIS_IRAWOBJECT_H

#include <thrift/transport/TBufferTransports.h>
#include "IObject.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {
                class IRawObject : public IObject {
                public:

                    static const std::string TYPE;

                    IRawObject(const std::shared_ptr<apache::thrift::transport::TTransport> &transport,
                               int8_t compression);

                    virtual std::shared_ptr<iterator::ICoreReadIterator<Any>> readIterator() override;

                    virtual std::shared_ptr<iterator::ICoreWriteIterator<Any>> writeIterator() override;

                    virtual void read(std::shared_ptr<transport::TTransport> trans) override;

                    virtual void write(std::shared_ptr<transport::TTransport> trans, int8_t compression) override;

                    virtual void copyFrom(IObject &source) override;

                    virtual void moveFrom(IObject &source) override;

                    virtual size_t getSize() override;

                    virtual std::string getType() override;

                    virtual ~IRawObject();

                protected:

                    IRawObject(const std::shared_ptr<apache::thrift::transport::TTransport> &transport,
                               int8_t compression, size_t elems, int8_t type);

                    inline void readHeader(std::shared_ptr<transport::TTransport> transport);

                    inline void writeHeader(std::shared_ptr<transport::TTransport> transport);

                    std::shared_ptr<transport::TTransport> transport;
                    size_t elems;
                    int8_t compression;
                    int8_t type;
                };
            }
        }
    }
}

#endif
