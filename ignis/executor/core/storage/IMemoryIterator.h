
#ifndef IGNIS_IMEMORYITERATOR_H
#define IGNIS_IMEMORYITERATOR_H

#include "IObject.h"
#include "../../../data/IManager.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {
                class IReadMemoryIterator : public ICoreReadIterator<IObject::Any> {
                public:

                    IReadMemoryIterator(std::vector<IObject::Any> *data,
                                        const std::shared_ptr<data::IManager<IObject::Any>> &manager);

                    IObject::Any &next() override;

                    bool hashNext() override;

                    bool isMoved() override;

                    void skip(size_t elems) override;

                    ~IReadMemoryIterator() override;

                private:
                    std::vector<IObject::Any> *data;
                    std::shared_ptr<data::serialization::IClassManagerType<std::vector<IObject::Any>>> class_manager;
                    size_t elems;
                    size_t pos;
                };

                class IWriteMemoryIterator : public ICoreWriteIterator<IObject::Any> {
                public:

                    IWriteMemoryIterator(std::vector<IObject::Any> *data,
                                         const std::shared_ptr<data::IManager<IObject::Any>> &manager);

                    void write(IObject::Any &obj) override;

                    void write(IObject::Any &&obj) override;

                    ~IWriteMemoryIterator() override;

                private:
                    std::vector<IObject::Any> *data;
                    std::shared_ptr<data::serialization::IClassManagerType<std::vector<IObject::Any>>> class_manager;
                };
            }
        }
    }
}
#endif
