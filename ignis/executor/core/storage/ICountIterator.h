
#ifndef IGNIS_ICOUNTITERATOR_H
#define IGNIS_ICOUNTITERATOR_H

#include "IObject.h"
#include "ICoreIterator.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {

                class IReadCountIterator : public ICoreReadIterator<IObject::Any> {
                public:
                    IReadCountIterator(size_t count, const std::shared_ptr<ICoreReadIterator<IObject::Any>> &iterator)
                            : count(count), iterator(iterator) {}

                    inline IObject::Any &next() override {
                        count--;
                        return iterator->next();
                    }

                    inline bool hashNext() override {
                        return count > 0 && iterator->hashNext();
                    }

                    inline bool isMoved() override {
                        return iterator->isMoved();
                    }

                    virtual ~IReadCountIterator() {}

                private:
                    size_t count;
                    std::shared_ptr<ICoreReadIterator<IObject::Any>> iterator;
                };
            }
        }
    }
}

#endif
