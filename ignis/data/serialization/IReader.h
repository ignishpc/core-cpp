
#ifndef UNTITLED_IREADER_H
#define UNTITLED_IREADER_H

#include <memory>
#include "IEnumTypes.h"

namespace ignis {
    namespace data {
        namespace serialization {

            class IReader {
            public:

            private:
                __int8_t read();
                size_t hash();
            };
        }
    }
}

#endif //UNTITLED_IREADER_H
