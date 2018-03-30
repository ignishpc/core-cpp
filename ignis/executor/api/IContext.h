
#ifndef IGNIS_IGNISCONTEXT_H
#define IGNIS_IGNISCONTEXT_H

#include <memory>
#include "../../data/serialization/IReader.h"
#include "../../data/serialization/IWriter.h"

namespace ignis {
    namespace executor {
        namespace api {
            class IContext {
            public:
            private:
                std::shared_ptr<data::serialization::IReader> object_reader;
                std::shared_ptr<data::serialization::IWriter> object_writer;
            };
        }
    }
}

#endif
