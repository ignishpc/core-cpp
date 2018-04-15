
#ifndef IGNIS_IRAWOBJECT_H
#define IGNIS_IRAWOBJECT_H

#include <memory>
#include <thrift/transport/TBufferTransports.h>
#include "IObject.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {
                class IRawObject : public IObject{
                public:

                    IRawObject(const std::shared_ptr<apache::thrift::transport::TTransport> &transport, int8_t compression);

                    virtual ~IRawObject();

                    virtual std::shared_ptr<api::IReadIterator<Any>> readIterator() override;

                    virtual std::shared_ptr<api::IWriteIterator<Any>> writeIterator() override;

                    virtual void read(std::shared_ptr<apache::thrift::transport::TTransport> trans) override;

                    virtual void write(std::shared_ptr<apache::thrift::transport::TTransport> trans, int8_t compression) override;

                protected:

                    inline virtual bool fastWrite(std::shared_ptr<apache::thrift::transport::TTransport> transport);

                    inline void readHeader(std::shared_ptr<apache::thrift::transport::TTransport> transport);

                    inline void writeHeader(std::shared_ptr<apache::thrift::transport::TTransport> transport);

                    std::shared_ptr<apache::thrift::transport::TTransport> transport;
                    size_t elems;
                    int8_t compression;
                };
            }
        }
    }
}

#endif
