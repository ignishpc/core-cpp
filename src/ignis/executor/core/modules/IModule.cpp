
#include "IModule.h"
#include "ignis/executor/core/RTTInfo.h"
#include "ignis/executor/core/exception/ILogicError.h"
#include "ignis/executor/core/io/IReader.h"
#include "ignis/executor/core/protocol/IObjectProtocol.h"
#include "ignis/executor/core/storage/IVoidPartition.h"
#include "ignis/executor/core/transport/IMemoryBuffer.h"

using namespace ignis::executor::core;
using namespace ignis::executor::core::modules;

IModule::IModule(std::shared_ptr<IExecutorData> &executor_data)
    : executor_data(executor_data), dynamic_types(executor_data->getProperties()) {}

IModule::~IModule() {}

std::shared_ptr<ignis::executor::core::selector::ITypeSelector> IModule::typeFromPartition() {
    IGNIS_LOG(info) << "Module: Cheeking partition type";
    auto group = executor_data->getPartitions<int>(true);
    auto type = group->elemType();
    if (group->partitions() > 0 && (*group)[0]->type() == storage::IVoidPartition::TYPE) {
        auto voidParts = executor_data->getPartitions<int>(true);
        std::string typeName;
        IGNIS_LOG(warning) << "Module: Forced to search type in binary objects, it must be slow in some case, "
                              "use src parameter in function to avoid";
        for (auto part : *voidParts) {
            if (part->bytes() > 0) {
                auto &voidPart = reinterpret_cast<storage::IVoidPartition &>(*part);
                auto transport = voidPart.readTransport();
                typeName = dynamic_types.typeFromBytes(transport);
                if (!typeName.empty()){
                    break;
                }
            }
        }
        auto type = executor_data->getType(typeName);
        if (type) { return type; }
        if (!typeName.empty()) { return compileFromName(typeName); }
        throw exception::ILogicError("The C ++ executor cannot identify the type of this partition");
    }
    return typeFromName(type.getStandardName());
}

std::shared_ptr<selector::ITypeSelector> IModule::typeFromName(const std::string &name) {
    auto type = executor_data->getType(name);
    if (!type) {
        IGNIS_LOG(warning)
                << "Type '" + name +
                           "' is not in the registry, that means  that the type is not compiled in the executor. "
                           "By default only the combinations of the most used types are compiled, the rest are loaded "
                           "from the libraries used by the user.";
        if (!name.empty()) { return compileFromName(name); }
    }

    return type;
}

std::shared_ptr<selector::ITypeSelector> IModule::typeFromSource(const ignis::rpc::ISource &source) {
    auto &name = source.obj.name;
    if (!name.empty() && name[0] == ':') { return typeFromName(name.substr(1)); }
    auto lib = executor_data->loadLibrary(source);
    if (lib->args.empty()) { throw exception::ILogicError("Function " + name + " has not type to use"); }
    auto type = lib->args[0];
    if (lib->args.size() > 1) {
        IGNIS_LOG(warning) << "Module: Function " << name << " has more than one type, using"
                           << type->info().getStandardName();
    }
    lib->loadClass(executor_data->getContext());
    return type;
}

std::shared_ptr<selector::ITypeSelector> IModule::compileFromName(const std::string &name) {
    IGNIS_LOG(warning) << "Module:  Type found but it is not compiled, now the executor tries to compile it. "
                          "The compilation process must be slow in some case, "
                          "the result will be stored in the job folder. "
                          "Use src parameter in function to avoid";
    auto lib = dynamic_types.compileType(name);
    IGNIS_LOG(info) << "Module: Compilation successful, use '" + lib +
                               "' as src parameter in function to avoid future recompilation";
    rpc::ISource source;
    source.obj.__set_name(lib);
    return typeFromSource(source);
}
