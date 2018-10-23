
#ifndef IGNIS_IMEMORYITERATOR_H
#define IGNIS_IMEMORYITERATOR_H

#include "../IObject.h"
#include "../../../api/IManager.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {
                namespace iterator {
                    class IReadMemoryIterator : public ICoreReadIterator<IObject::Any> {
                    public:

                        IReadMemoryIterator(api::ICollectionManager<IObject::Any>::Class* data,
                                            const std::shared_ptr<api::ICollectionManager<IObject::Any>> &collection_manager);

                        IObject::Any &next() override;

                        bool hasNext() override;

                        bool isMoved() override;

                        void skip(size_t elems) override;

                        ~IReadMemoryIterator() override;

                    private:
                        api::ICollectionManager<IObject::Any>::Class* data;
                        std::shared_ptr<api::ICollectionManager<IObject::Any>> collection_manager;
                        size_t pos;
                        size_t size;
                    };

                    class IWriteMemoryIterator : public ICoreWriteIterator<IObject::Any> {
                    public:

                        IWriteMemoryIterator(api::ICollectionManager<IObject::Any>::Class* data,
                                             const std::shared_ptr<api::ICollectionManager<IObject::Any>> &collection_manager);

                        void write(IObject::Any &obj) override;

                        void write(IObject::Any &&obj) override;

                        ~IWriteMemoryIterator() override;

                    private:
                        api::ICollectionManager<IObject::Any>::Class* data;
                        std::shared_ptr<api::ICollectionManager<IObject::Any>> collection_manager;
                    };
                }
            }
        }
    }
}
#endif
