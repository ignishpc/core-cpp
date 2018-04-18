
#include "IDinamicObject.h"
#include <dlfcn.h>
#include "../../exceptions/IInvalidArgument.h"
#include "../../data/RTTInfo.h"

using namespace std;
using namespace ignis::executor::core;

ObjectLoader::ObjectLoader(const std::string &name) {
    int sep = name.find(':');

    if (sep < -1) {
        throw exceptions::IInvalidArgument(name + " is a invalid c++ class");
    }
    string path = name.substr(0, sep);
    string class_name = name.substr(sep + 1, name.size());

    library = dlopen(path.c_str(), RTLD_NOW | RTLD_GLOBAL);

    if (!library) {
        throw exceptions::IInvalidArgument(path + " was not found or could not be loaded");
    }

    auto constructor = (void *(*)()) dlsym(library, (class_name + "_constructor").c_str());
    destructor = (void (*)(void *)) dlsym(library, (class_name + "_destructor").c_str());

    if (!constructor || !destructor) {
        throw exceptions::IInvalidArgument(
                class_name + " must be register, use: ignis_register_class(name, class) in you library");
    }

    object = (*constructor)();
}

ObjectLoader::~ObjectLoader() {
    try {
        (*destructor)(object);
    } catch (...) {}
    dlclose(library);
}

void *ObjectLoader::getObject() {
    return object;
}
