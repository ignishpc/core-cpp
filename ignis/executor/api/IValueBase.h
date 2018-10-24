
#ifndef IGNIS_IVALUEBASE_H
#define IGNIS_IVALUEBASE_H

#include "IRegister.h"
#include "IManager.h"

namespace ignis {
    namespace executor {
        namespace api {

            template<typename T>
            class IValueBase {
            private:
                Type_ptr<IManager<T>> var_type_t;
            public:
                decltype(var_type_t) type_t() { return var_type_t; }
            };

        }
    }
}

#endif
