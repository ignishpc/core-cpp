#include "ILibraryLoader.h"
#include "exception/IInvalidArgument.h"
#include <dlfcn.h>
#include <ghc/filesystem.hpp>
#include <ignis/executor/core/selector/ISelector.h>


using namespace ignis::executor::core;

std::shared_ptr<selector::ISelectorGroup> ILibraryLoader::loadFunction(const std::string &name) {
    int sep = name.find(':');
    if (sep == std::string::npos) { throw exception::IInvalidArgument(name + " is not a valid c++ class"); }

    std::string path = name.substr(0, sep);
    std::string class_name = name.substr(sep + 1, name.size());

    if (!ghc::filesystem::exists(path)) { throw exception::IInvalidArgument(path + " was not found"); }

    void *library = dlopen(path.c_str(), RTLD_NOW | RTLD_GLOBAL | RTLD_DEEPBIND);

    if (!library) { throw exception::IInvalidArgument(path + " could not be loaded: " + dlerror()); }

    auto constructor = (selector::ISelectorGroup * (*) ()) dlsym(library, (class_name + "_constructor").c_str());
    auto destructor = (void (*)(selector::ISelectorGroup *)) dlsym(library, (class_name + "_destructor").c_str());

    if (!constructor || !destructor) {
        dlclose(library);
        throw exception::IInvalidArgument(class_name +
                                          " must be register, use: ignis_register(name, class) in you library");
    }

    auto object = (*constructor)();

    return std::shared_ptr<selector::ISelectorGroup>(object, [library, destructor](selector::ISelectorGroup *object) {
        (*destructor)(object);
        dlclose(library);
    });
}

std::vector<std::string> ILibraryLoader::loadLibrary(const std::string &path) {
    if (!ghc::filesystem::exists(path)) { throw exception::IInvalidArgument(path + " was not found"); }

    void *library = dlopen(path.c_str(), RTLD_NOW | RTLD_GLOBAL | RTLD_DEEPBIND);

    if (!library) { throw exception::IInvalidArgument(path + " could not be loaded: " + dlerror()); }

    const char **__ignis_library__ = (const char **) dlsym(library, "__ignis_library__");

    if (!__ignis_library__) {
        dlclose(library);
        throw exception::IInvalidArgument(path + " must use create_ignis_library to be loaded as library");
    }

    std::vector<std::string> functions;

    for (int i = 0; __ignis_library__[i] != nullptr; i++) {
        functions.push_back(path + ":" + std::string(__ignis_library__[i]));
    }
    dlclose(library);

    return functions;
}
