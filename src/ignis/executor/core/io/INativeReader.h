
#ifndef IGNIS_INATIVEREADER_H
#define IGNIS_INATIVEREADER_H

#include "IReader.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace io {

                template<typename T>
                struct INativeReaderType;

                template<typename T>
                class INativeReader {
                public:

                    virtual inline T operator()(protocol::IProtocol &protocol) {
                        return reader(protocol);
                    }

                    virtual inline void operator()(protocol::IProtocol &protocol, T& obj) {
                        reader(protocol, obj);
                    }

                private:
                    INativeReaderType<T> reader;

                };
            }
        }
    }
}

#include "INativeReader.tcc"

#endif
