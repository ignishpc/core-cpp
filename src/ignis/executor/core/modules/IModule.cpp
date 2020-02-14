
#include "IModule.h"
#include "ignis/executor/core/transport/IMemoryBuffer.h"
#include "ignis/executor/core/protocol/IObjectProtocol.h"
#include "ignis/executor/core/io/IReader.h"
#include "ignis/executor/core/RTTInfo.h"
#include "ignis/executor/core/exception/ILogicError.h"

using namespace ignis::executor::core;
using namespace ignis::executor::core::modules;

IModule::IModule(std::shared_ptr<IExecutorData> &executor_data) : executor_data(executor_data) {}

IModule::~IModule() {}

std::shared_ptr<ignis::executor::core::selector::ITypeSelector> IModule::typeFromPartition() {
    IGNIS_LOG(info) << "Cheeking partition type";
    auto type = executor_data->getPartitions<int>(false)->elemType();
    if (type.isVoid()) {
        throw exception::ILogicError("The C ++ executor cannot identify the type of this partition");
    }
    return typeFromName(type.getStandardName());
}

std::shared_ptr<ignis::executor::core::selector::ITypeSelector> IModule::typeFromHeader(const std::string &header) {
    const int N_NAMED_TYPES = 8;
    const static std::string BASIC_NAMED_TYPES[]{
            RTTInfo::from<void>().getStandardName(),//I_VOID = 0x0,
            RTTInfo::from<bool>().getStandardName(),//I_BOOL = 0x1,
            RTTInfo::from<int8_t>().getStandardName(),//I_I08 = 0x2,
            RTTInfo::from<int16_t>().getStandardName(),//I_I16 = 0x3,
            RTTInfo::from<int32_t>().getStandardName(),//I_I32 = 0x4,
            RTTInfo::from<int64_t>().getStandardName(),//I_I64 = 0x5,
            RTTInfo::from<double>().getStandardName(),//I_DOUBLE = 0x6,
            RTTInfo::from<std::string>().getStandardName(),//I_STRING = 0x7,
    };
    IGNIS_LOG(info) << "Cheeking incoming partition type";
    std::string type_name = "";
    try {
        auto buffer = std::make_shared<transport::IMemoryBuffer>((uint8_t *) const_cast<char *>(header.c_str()),
                                                                 header.size());
        auto proto = std::make_shared<protocol::IObjectProtocol>(buffer);
        bool native;
        proto->readBool(native);
        auto tp = io::readTypeAux(*proto);
        auto sz = io::readSizeAux(*proto);

        if (tp == io::IEnumTypes::I_LIST) {
            auto elem_tp = io::readTypeAux(*proto);
            if (0 < elem_tp && elem_tp < N_NAMED_TYPES) {
                type_name = BASIC_NAMED_TYPES[elem_tp];
            }
        } else if (tp == io::IEnumTypes::I_PAIR_LIST) {
            auto first_tp = io::readTypeAux(*proto);
            auto second_tp = io::readTypeAux(*proto);
            if (0 < first_tp && first_tp < N_NAMED_TYPES && 0 < second_tp && second_tp < N_NAMED_TYPES) {
                type_name = "std::pair<" + BASIC_NAMED_TYPES[first_tp] + "" + BASIC_NAMED_TYPES[second_tp] + ">";
            }
        } else if (tp == io::IEnumTypes::I_BINARY) {
            type_name = RTTInfo::from<uint8_t>().getStandardName();
        }
    } catch (std::exception &ex) {
        IGNIS_LOG(warning) << "exception: " << ex.what();
    }

    if (type_name.empty()) {
        throw exception::ILogicError("Incoming partition type not found or is too complex.");
    } else {
        IGNIS_LOG(info) << "Partition type found: " << type_name;
    }

    return typeFromName(type_name);
}

std::shared_ptr<selector::ITypeSelector> IModule::typeFromName(const std::string &name) {
    auto type = executor_data->getType(name);
    if (!type) {
        throw exception::ILogicError("Type '" + name +
                                     "' is not in the registry, that means  that the type is not compiled in the executor. "
                                     "By default only the combinations of the most used types are compiled, the rest are loaded "
                                     "from the libraries used by the user.");
    }

    return type;
}

std::shared_ptr<selector::ITypeSelector> IModule::typeFromSource(const ignis::rpc::ISource &source) {
    auto &name = source.obj.name;
    if (!name.empty() && name[0] == ':') {
        return typeFromName(name.substr(1));
    }
    auto lib = executor_data->loadLibrary(source);
    if (lib->args.empty()) {
        throw exception::ILogicError("Function " + name + " has not type to use");
    }
    auto type = lib->args.begin()->second;
    if (lib->args.size() > 1) {
        IGNIS_LOG(warning) << "Function " << name << " has more than one type, using" << type->info().getStandardName();
    }
    return type;
}
