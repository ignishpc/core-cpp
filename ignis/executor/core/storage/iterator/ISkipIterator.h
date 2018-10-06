
#ifndef IGNIS_ISKIPTITERATOR_H
#define IGNIS_ISKIPTITERATOR_H

#include "../IObject.h"
#include "ICoreIterator.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {
                namespace iterator {
                    class IReadSkipIterator : public ICoreReadIterator<IObject::Any> {
                    public:
                        IReadSkipIterator(size_t skip, const std::shared_ptr<ICoreReadIterator<IObject::Any>> &iterator)
                                : skip(skip), iterator(iterator) {}

                        inline IObject::Any &next() override {
                            return iterator->next();
                        }

                        inline bool hashNext() override {
                            while (skip > 0 && iterator->hashNext()) {
                                iterator->next();
                            }
                            return iterator->hashNext();
                        }

                        inline bool isMoved() override {
                            return iterator->isMoved();
                        }

                        virtual ~IReadSkipIterator() {}

                    private:
                        size_t skip;
                        std::shared_ptr<ICoreReadIterator<IObject::Any>> iterator;
                    };
                }
            }
        }
    }
}

#endif
