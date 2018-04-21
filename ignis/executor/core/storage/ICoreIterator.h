
#ifndef IGNIS_ICOREITERATOR_H
#define IGNIS_ICOREITERATOR_H


#include "../../api/IReadIterator.h"
#include "../../api/IWriteIterator.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {

                template<typename T>
                class ICoreReadIterator : public api::IReadIterator<T> {
                public:

                    virtual T &next() = 0;

                    virtual bool hashNext() = 0;

                    virtual bool isMoved() = 0;

                    virtual ~ICoreReadIterator() {};
                };

                template<typename T>
                class ICoreWriteIterator : public api::IWriteIterator<T> {
                public:

                    virtual void write(T &obj) = 0;

                    virtual void write(T &&obj) = 0;

                    virtual ~ICoreWriteIterator() {};
                };
            }
        }
    }
}

#endif
