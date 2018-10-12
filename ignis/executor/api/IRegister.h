
#ifndef IGNIS_IREGISTER_H
#define IGNIS_IREGISTER_H

#define ignis_register_class(name, class_name)\
extern "C" class_name* name##_constructor(){\
    return new class_name();\
}\
\
extern "C" void name##_destructor(class_name* obj){\
    delete obj;\
}
#endif
