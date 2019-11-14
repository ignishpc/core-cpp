
#ifndef IGNIS_IHEADERTRANSPORT_H
#define IGNIS_IHEADERTRANSPORT_H

#include "ITransport.h"
#include <memory>

namespace ignis {
    namespace executor {
        namespace core {
            namespace transport {
                class IHeaderTransport
                        : public apache::thrift::transport::TVirtualTransport<IHeaderTransport, ITransport> {
                public:

                    IHeaderTransport(std::shared_ptr<transport::ITransport> &trans, std::string &header);

                    virtual ~IHeaderTransport();

                    uint32_t read(uint8_t *buf, uint32_t len);

                    void write(const uint8_t *buf, uint32_t len);

                    const uint8_t *borrow(uint8_t *buf, uint32_t *len);

                    void consume(uint32_t len);

                private:
                    std::shared_ptr<transport::ITransport> trans;
                    std::string header;
                    int pos;
                };
            }
        }
    }
}

#endif

