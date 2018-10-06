
#ifndef IGNIS_ILOGITERATOR_H
#define IGNIS_ILOGITERATOR_H

#include "../IObject.h"
#include "ICoreIterator.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {
                namespace iterator {
                    class IReadLogIterator : public ICoreReadIterator<IObject::Any> {
                    public:
                        IReadLogIterator(const std::shared_ptr<ICoreReadIterator<IObject::Any>> &reader,
                                         const size_t trigger_size,
                                         const std::string &msg_before, const std::string &msg_after,
                                         size_t total_size = 0);

                        IObject::Any &next() override;

                        inline bool hashNext() override {
                            reader->hashNext();
                        }

                        inline bool isMoved() override {
                            return reader->isMoved();
                        }

                    private:
                        void inline printLog();

                        std::shared_ptr<ICoreReadIterator<IObject::Any>> reader;
                        std::string msg_before;
                        std::string msg_after;
                        size_t trigger_size;
                        size_t read_size;
                        size_t total_size;
                    };
                }
            }
        }
    }
}
#endif
