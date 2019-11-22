
#include "ILibraryLoader.h"
#include <dlfcn.h>
#include "exception/IInvalidArgument.h"
#include <boost/filesystem.hpp>

using namespace ignis::executor::core;

std::shared_ptr<void> ILibraryLoader::vload(const std::string &name) {
    int sep = name.find(':');
    if (sep < -1) {
        throw exception::IInvalidArgument(name + " is not a valid c++ class");
    }

    std::string path = name.substr(0, sep);
    std::string class_name = name.substr(sep + 1, name.size());

    if (!boost::filesystem::exists(path)) {
        throw exception::IInvalidArgument(path + " was not found");
    }

    void *library = dlopen(path.c_str(), RTLD_NOW | RTLD_GLOBAL | RTLD_DEEPBIND);

    if (!library) {
        throw exception::IInvalidArgument(path + " could not be loaded");
    }

    auto constructor = (void *(*)()) dlsym(library, (class_name + "_constructor").c_str());
    auto destructor = (void (*)(void *)) dlsym(library, (class_name + "_destructor").c_str());

    if (!constructor || !destructor) {
        throw exception::IInvalidArgument(
                class_name + " must be register, use: ignis_register(name, class) in you library");
    }

    auto object = (*constructor)();

    return std::shared_ptr<void>(object, [library, destructor](void *object) {
        (*destructor)(object);
        dlclose(library);
    });
}




