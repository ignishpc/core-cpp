
#ifndef IGNIS_ICOREITERATOR_H
#define IGNIS_ICOREITERATOR_H

#include "../../../../IHeaders.h"
#include "../../../api/IReadIterator.h"
#include "../../../api/IWriteIterator.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {
                namespace iterator {

                    template<typename T>
                    class ICoreReadIterator : public api::IReadIterator<T> {
                    public:

                        virtual T &next() = 0;

                        virtual std::shared_ptr<T> nextShared() = 0;

                        virtual bool hasNext() = 0;

                        virtual bool isMoved() = 0;

                        virtual void skip(size_t elems) { while (elems-- > 0 && hasNext()) { next(); }}

                        virtual ~ICoreReadIterator() {};
                    };

                    template<typename T>
                    class ICoreWriteIterator : public api::IWriteIterator<T> {
                    public:

                        virtual void write(T &obj) = 0;

                        virtual void write(T &&obj) = 0;

                        virtual ~ICoreWriteIterator() {};
                    };

                    template<typename T>
                    inline void
                    readToWrite(ICoreReadIterator<T> &reader, ICoreWriteIterator<T> &writer, bool force_move = false) {
                        if (reader.isMoved() || force_move) {
                            while (reader.hasNext()) { writer.write((T &&) reader.next()); }
                        } else {
                            while (reader.hasNext()) { writer.write(reader.next()); }
                        }
                    }

                    template<typename T>
                    inline void readToWrite(ICoreReadIterator<T> &reader, ICoreWriteIterator<T> &writer, size_t n,
                                            bool force_move = false) {
                        if (reader.isMoved() || force_move) {
                            for (; reader.hasNext() && n > 0; n--) { writer.write((T &&) reader.next()); }
                        } else {
                            for (; reader.hasNext() && n > 0; n--) { writer.write(reader.next()); }
                        }
                    }

                }
            }
        }
    }
}

#endif
