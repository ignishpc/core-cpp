
#ifndef IGNIS_IMEMORYITERATOR_H
#define IGNIS_IMEMORYITERATOR_H

#include "IObject.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {
                class IReadMemoryIterator : public api::IReadIterator<IObject::Any> {
                public:

                    IReadMemoryIterator(std::vector<IObject::Any> *data,
                                        const std::shared_ptr<data::serialization::IColectionHandle<IObject::Any>> &data_handle);

                    IObject::Any &next() override;

                    bool hashNext() override;

                    ~IReadMemoryIterator() override;
                private:
                    std::vector<IObject::Any> *data;
                    std::shared_ptr<data::serialization::IColectionHandle<IObject::Any>> data_handle;
                    size_t elems;
                    size_t pos;
                };

                class IWriteMemoryIterator : public api::IWriteIterator<IObject::Any> {
                public:

                    IWriteMemoryIterator(std::vector<IObject::Any> *data,
                                         const std::shared_ptr<data::serialization::IColectionHandle<IObject::Any>> &data_handle);

                    void write(IObject::Any &obj) override;

                    void write(IObject::Any &&obj) override;

                    ~IWriteMemoryIterator() override;
                private:
                    std::vector<IObject::Any> *data;
                    std::shared_ptr<data::serialization::IColectionHandle<IObject::Any>> data_handle;
                };
            }
        }
    }
}
#endif
