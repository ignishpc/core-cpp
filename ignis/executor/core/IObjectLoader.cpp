
#include "IObjectLoader.h"
#include <dlfcn.h>
#include <boost/filesystem.hpp>
#include "../../exceptions/IInvalidArgument.h"
#include "../../data/IMemoryBuffer.h"
#include "../../data/IObjectProtocol.h"
#include <iostream>

using namespace ignis::executor::api;
using namespace ignis::executor::core;

std::shared_ptr<void> IObjectLoader::vload(const std::string &name) {
    int sep = name.find(':');
    if (sep < -1) {
        throw exceptions::IInvalidArgument(name + " is not a valid c++ class");
    }
    if (libraries.find(name) != libraries.end()) {
        return libraries[name];
    }

    std::string path = name.substr(0, sep);
    std::string class_name = name.substr(sep + 1, name.size());

    if (!boost::filesystem::exists(path)) {
        throw exceptions::IInvalidArgument(path + " was not found");
    }

    void *library = dlopen(path.c_str(), RTLD_NOW | RTLD_GLOBAL | RTLD_DEEPBIND);

    if (!library) {
        throw exceptions::IInvalidArgument(path + " could not be loaded");
    }

    auto constructor = (void *(*)()) dlsym(library, (class_name + "_constructor").c_str());
    auto destructor = (void (*)(void *)) dlsym(library, (class_name + "_destructor").c_str());

    if (!constructor || !destructor) {
        throw exceptions::IInvalidArgument(
                class_name + " must be register, use: ignis_register_class(name, class) in you library");
    }

    auto object = (*constructor)();

    return libraries[path] = std::shared_ptr<void>(object, [library, destructor](void *object) {
        (*destructor)(object);
        dlclose(library);
    });
}


std::shared_ptr<storage::IObject::Any> IObjectLoader::loadVariable(const std::string &bytes,
                                                                   std::shared_ptr<executor::api::IManager<storage::IObject::Any>> manager) {

    auto buffer = std::make_shared<data::IMemoryBuffer>((uint8_t *) bytes.c_str(),
                                                        (size_t) (bytes.end() - bytes.begin()));
    for (int i = 0; i < (size_t) (bytes.end() - bytes.begin()); i++) {
        std::cout << *((uint8_t *)(&bytes.c_str()[i])) << " ";
    }
    std::cout << std::endl;

    data::IObjectProtocol proto(buffer);
    return proto.readObject(*manager->reader(), manager->deleter());
}


std::shared_ptr<IManager<storage::IObject::Any>> IObjectLoader::getManager(const std::string &id) {

}

std::shared_ptr<IManager<storage::IObject::Any>> IObjectLoader::compileManager(const std::string &id) {

}

void IObjectLoader::registermanager(std::shared_ptr<executor::api::IManager<storage::IObject::Any>> manager) {

}

bool IObjectLoader::containsManager(const std::string &id) {
    return false;
}

std::string IObjectLoader::parseId(storage::IObject &obj) {
    return "";
}

std::string IObjectLoader::parseId(api::IManager<storage::IObject::Any> &manager) {

}

