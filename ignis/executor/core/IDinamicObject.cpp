
#include "IDinamicObject.h"
#include <dlfcn.h>
#include <stdexcept>
#include "../../data/RTTInfo.h"

using namespace std;
using namespace ignis::executor::core;

void ObjectLoader::open(IDinamicObject<api::ILoadClass<void>> &obj, std::string &name, const std::type_info &info) {
    int sep = name.find(':');

    if (sep < -1) {
        throw invalid_argument(name + " is a invalid c++ class");
    }
    string path = name.substr(0, sep);
    string class_name = name.substr(sep + 1, name.size());

    void *library = dlopen(path.c_str(), RTLD_NOW | RTLD_GLOBAL);

    if (!library) {
        throw invalid_argument(path + " was not found or could not be loaded");
    }

    auto constructor = (api::ILoadClass<void> *(*)()) dlsym(library, (class_name + "_constructor").c_str());
    auto destructor = (void (*)(api::ILoadClass<void> *)) dlsym(library, (class_name + "_destructor").c_str());

    if (!constructor || !destructor) {
        throw invalid_argument(
                class_name + " must be register, use: ignis_register_class(class_name) after closing your class");
    }

    auto object = (*constructor)();
    if (object->id != info) {
        string id_name = data::RTTInfo(object->id).getStandardName();
        try {
            (*destructor)(object);
        } catch (...) {}
        dlclose(library);
        throw invalid_argument(class_name + " invalid, expected (" + data::RTTInfo(info).getStandardName() +
                               ") found (" + id_name + ")");
    }

    obj.handle = make_shared<IDinamicObject<api::ILoadClass<void>>::Handle>(object, destructor, library);
}

void ObjectLoader::close(void *dl) {
    dlclose(dl);
}