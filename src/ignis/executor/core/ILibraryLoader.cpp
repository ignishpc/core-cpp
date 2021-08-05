
#include "ILibraryLoader.h"
#include "exception/IInvalidArgument.h"
#include <boost/interprocess/sync/file_lock.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <dlfcn.h>
#include <fstream>
#include <functional>
#include <ghc/filesystem.hpp>
#include <ignis/executor/core/selector/ISelector.h>


using namespace ignis::executor::core;

ILibraryLoader::ILibraryLoader(IPropertyParser &properties) : properties(properties) {}

std::shared_ptr<selector::ISelectorGroup> ILibraryLoader::loadFunction(const std::string &name2) {
    std::string name = name2;
    if (name.rfind("[]", 0) == 0) { name = loadLambda(name); }
    int sep = name.find(':');
    if (sep == std::string::npos) { throw exception::IInvalidArgument(name + " is not a valid c++ class"); }

    std::string path = name.substr(0, sep);
    std::string class_name = name.substr(sep + 1, name.size());

    if (!ghc::filesystem::exists(path)) { throw exception::IInvalidArgument(path + " was not found"); }

    void *library = dlopen(path.c_str(), RTLD_NOW | RTLD_GLOBAL | RTLD_DEEPBIND);

    if (!library) { throw exception::IInvalidArgument(path + " could not be loaded: " + dlerror()); }

    auto constructor = (selector::ISelectorGroup * (*) ()) dlsym(library, (class_name + "_constructor").c_str());
    auto destructor = (void(*)(selector::ISelectorGroup *)) dlsym(library, (class_name + "_destructor").c_str());

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

std::vector<std::string> ILibraryLoader::loadLibrary(const std::string &path2) {
    std::string path = path2;
    if (path.rfind("c++", 0) != std::string::npos) { path = loadSource(path); }
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


std::string ILibraryLoader::compile(const std::string &str) {
    if (std::system("g++ --version") == 0) { throw exception::ILogicError("g++ compiler not found in path"); }
    std::string folder = properties.jobDirectory() + "/cppsrc";
    if (!ghc::filesystem::is_directory(folder)) {
        std::error_code error;
        ghc::filesystem::create_directories(folder, error);
        if (error && !ghc::filesystem::is_directory(folder)) {
            throw exception::IInvalidArgument("Unable to create directory " + folder + " " + error.message());
        }
    }

    boost::interprocess::file_lock lock(folder.c_str());
    boost::interprocess::scoped_lock<boost::interprocess::file_lock> slock(lock);
    auto hash = std::to_string(std::hash<std::string>()(str));
    std::string id;
    std::string fileSource;
    std::string fileLibrary;

    int i = 100;
    do {
        id = hash + std::to_string(i);
        fileSource = folder + "/" + id + ".cpp";
        fileLibrary = folder + "/" + id + ".so";

        if (ghc::filesystem::exists(fileSource)) {
            std::string content;
            std::getline(std::ifstream(fileSource), content, '\0');
            if (content == str) { return fileLibrary; }
        }
    } while (ghc::filesystem::exists(fileSource));


    std::ofstream(fileSource, std::ifstream::out | std::ifstream::binary | std::fstream::trunc) << str;
    std::string headers = std::string(std::getenv("IGNIS_HOME")) + "/core/cpp/include";
    std::string cmd = "g++ -I " + headers + " -g -O3 -shared -fPIC -o " + fileLibrary + " " + fileSource;

    int error = std::system(cmd.c_str());
    if (error != 0) {
        throw exception::ILogicError("g++ exited with exit value " + std::to_string(error) + " " + fileSource);
    }

    return fileLibrary;
}

std::string ILibraryLoader::loadLambda(const std::string &str) {
    auto sourceCode = std::string() +
                      "#include <ignis/executor/core/ILambda.h>"
                      "auto lambda =" + str + ";\n"
                      "ignis_lambda_export(lambda)\n";
    return "Lambda:" + compile(sourceCode);
}

std::string ILibraryLoader::loadSource(const std::string &str) { return compile(str); }
