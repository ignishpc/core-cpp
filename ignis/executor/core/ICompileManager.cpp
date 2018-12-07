
#include "ICompileManager.h"
#include <boost/filesystem.hpp>
#include "ILog.h"

using namespace ignis::executor::core;

ICompileManager::ICompileManager(storage::IObject &obj) : obj(obj) {
}

std::string ICompileManager::compile() {
    if (std::system("g++ --version") == 0) {
        IGNIS_LOG(info) << "ICompileManager g++ found OK";
    } else {
        IGNIS_LOG(error) << "ICompileManager g++ not found";
        return "";
    }
    auto type = parseType();
    if (type.length() == 0) {
        return "";
    }

    std::string code = "#include<ignis/executor/api/IValue.h>\n"
                       "class CompileType : public IValue<" + type +
                       ">{};\n"
                       "ignis_register_class(CompileType,CompileType)";

    auto source = boost::filesystem::unique_path("ignis%%%%%%%%%%%%%%%%.cpp");
    auto lib = boost::filesystem::unique_path("libignis%%%%%%%%%%%%%%%%.so");
    if (std::system(("g++ -Wall -shared -fPIC -o " + lib.string() + " " + source.string()).data()) == 0) {
        return lib.string();
    }
    return "";
}

std::string ICompileManager::parseType() {
    IGNIS_LOG(error) << "ICompileManager No implemented yet";//TODO
    return "";
}

ICompileManager::~ICompileManager() {
}
