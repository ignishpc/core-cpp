
#ifndef IGNIS_ILOADCLASS_H
#define IGNIS_ILOADCLASS_H

#include <typeinfo>

#define ignis_register_class(name)\
extern "C"{\
    inline name* name##_constructor(){\
        return new name();\
    }\
    \
    inline void name##_destructor(name* obj){\
        delete obj;\
    }\
}

namespace ignis {
    namespace executor {
        namespace api {

            template <typename T>
            class ILoadClass {
            public:
                    const std::type_info& info = typeid(T);
            };
        }
    }
}

#endif
