
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

                    std::shared_ptr<ICoreReadIterator<Any>> readIterator() override;

                    std::shared_ptr<ICoreWriteIterator<Any>> writeIterator() override;

                    void read(std::shared_ptr<apache::thrift::transport::TTransport> trans) override;

                    void
                    write(std::shared_ptr<apache::thrift::transport::TTransport> trans, int8_t compression) override;

                    size_t getSize() override;

                    bool hasSize() override;

                    void clear() override;

                    void fit() override;

                    std::string getType() override;

                    std::shared_ptr<data::IManager<Any>> getManager() override;

                    void setManager(std::shared_ptr<data::IManager<Any>> manager) override;

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
