
#ifndef IGNIS_IREGISTER_H
#define IGNIS_IREGISTER_H

#define ignis_register_class(name, class)\
extern "C"{\
    inline name* name##_constructor(){\
        return new class();\
    }\
    \
    inline void name##_destructor(class* obj){\
        delete obj;\
    }\
}
#endif
