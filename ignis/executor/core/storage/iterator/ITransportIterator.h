
#ifndef IGNIS_ITRANSPORTITERATOR_H
#define IGNIS_ITRANSPORTITERATOR_H

#include "../../../../data/IObjectProtocol.h"
#include "../IObject.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {
                namespace iterator {
                    class IReadTransportIterator : public ICoreReadIterator<IObject::Any> {
                    public:

                        IReadTransportIterator(const std::shared_ptr<transport::TTransport> &transport,
                                               const std::shared_ptr<api::IManager<IObject::Any>> &manager,
                                               size_t& elems);

                        IObject::Any &next() override;

                        bool hashNext() override;

                        bool isMoved() override;

                        virtual ~IReadTransportIterator();

                    private:
                        std::shared_ptr<transport::TTransport> transport;
                        std::shared_ptr<data::IObjectProtocol> protocol;
                        std::shared_ptr<api::IManager<IObject::Any>> manager;
                        std::shared_ptr<data::handle::IReader<IObject::Any>> reader;
                        std::shared_ptr<IObject::DataHandle<IObject::Any>> actual;
                        size_t& elems;
                    };

                    class IWriteTransportIterator : public ICoreWriteIterator<IObject::Any> {
                    public:

                        IWriteTransportIterator(const std::shared_ptr<transport::TTransport> &transport,
                                                const std::shared_ptr<api::IManager<IObject::Any>> &manager,
                                                size_t &elems);

                        void write(IObject::Any &obj) override;

                        void write(IObject::Any &&obj) override;

                        virtual ~IWriteTransportIterator();

                    private:
                        std::shared_ptr<transport::TTransport> transport;
                        std::shared_ptr<data::IObjectProtocol> protocol;
                        std::shared_ptr<api::IManager<IObject::Any>> manager;
                        std::shared_ptr<data::handle::IWriter<IObject::Any>> writer;
                        size_t &elems;
                    };
                }
            }
        }
    }
}

#endif
