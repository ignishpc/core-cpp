
#ifndef IGNIS_ITHREADITERATOR_H
#define IGNIS_ITHREADITERATOR_H

#include <utility>
#include <mutex>
#include "IObject.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {

                class IThreadSplitter {
                public:
                    IThreadSplitter(IObject &object_in, IObject &object_out, size_t threads, size_t buffer, bool order);

                    std::pair<std::shared_ptr<iterator::ICoreReadIterator<IObject::Any>>,
                            std::shared_ptr<iterator::ICoreWriteIterator<IObject::Any>>> operator[](size_t pos);

                private:
                    IObject &object_in;
                    IObject &object_out;
                    std::mutex mutex_read;
                    std::mutex mutex_write;
                    std::vector<std::pair<std::shared_ptr<iterator::ICoreReadIterator<IObject::Any>>,
                            std::shared_ptr<iterator::ICoreWriteIterator<IObject::Any>>>> iterators;
                    size_t write_next;
                };
            }
        }
    }
}


#endif
