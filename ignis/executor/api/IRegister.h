
#ifndef IGNIS_IREGISTER_H
#define IGNIS_IREGISTER_H

#include <memory>

namespace ignis {
    namespace executor {
        namespace api {
            template <typename T>
            struct Type_ptr : std::shared_ptr<T>
            {
                Type_ptr() : std::shared_ptr<T>(new T()) {}
            };
        }
    }
}

#define ignis_register_class(name, class_name)\
extern "C" class_name* name##_constructor(){\
    return new class_name();\
}\
\
extern "C" void name##_destructor(class_name* obj){\
    delete obj;\
}
#endif
