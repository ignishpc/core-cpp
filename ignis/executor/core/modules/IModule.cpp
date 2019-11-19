
#include "IModule.h"
#include "executor/core/transport/IMemoryBuffer.h"
#include "executor/core/protocol/IObjectProtocol.h"
#include "executor/core/io/IReader.h"
#include "executor/core/RTTInfo.h"
#include "executor/core/exception/ILogicError.h"

using namespace ignis::executor::core;
using namespace ignis::executor::core::modules;

IModule::IModule(std::shared_ptr<IExecutorData> &executor_data) : executor_data(executor_data) {}

IModule::~IModule() {}

std::shared_ptr<ignis::executor::core::selector::ITypeSelector> IModule::typeFromPartition() {
    IGNIS_LOG(info) << "Cheeking partition type";
    auto type = executor_data->getPartitions<int>()->elemType();
    if (type.isVoid()) {
        throw exception::ILogicError("The C ++ executor cannot identify the type of this partition");
    }
    return executor_data->getType(type.getStandardName());
}

std::string BASIC_NAMED_TYPES[]{
        RTTInfo::from<void>().getStandardName(),//I_VOID = 0x0,
        RTTInfo::from<bool>().getStandardName(),//I_BOOL = 0x1,
        RTTInfo::from<int8_t>().getStandardName(),//I_I08 = 0x2,
        RTTInfo::from<int16_t>().getStandardName(),//I_I16 = 0x3,
        RTTInfo::from<int32_t>().getStandardName(),//I_I32 = 0x4,
        RTTInfo::from<int64_t>().getStandardName(),//I_I64 = 0x5,
        RTTInfo::from<double>().getStandardName(),//I_DOUBLE = 0x6,
        RTTInfo::from<std::string>().getStandardName(),//I_STRING = 0x7,
};

int N_NAMED_TYPES = 8;

std::shared_ptr<ignis::executor::core::selector::ITypeSelector> IModule::typeFromHeader(std::string &header) {
    IGNIS_LOG(info) << "Cheeking incoming partition type";
    std::string named_type = "";
    std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type;
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
                named_type = BASIC_NAMED_TYPES[elem_tp];
            }
        } else if (tp == io::IEnumTypes::I_PAIR_LIST) {
            auto first_tp = io::readTypeAux(*proto);
            auto second_tp = io::readTypeAux(*proto);
            if (0 < first_tp && first_tp < N_NAMED_TYPES && 0 < second_tp && second_tp < N_NAMED_TYPES) {
                named_type = "std::pair<" + BASIC_NAMED_TYPES[first_tp] + "" + BASIC_NAMED_TYPES[second_tp] + ">";
            }
        } else if (tp == io::IEnumTypes::I_BINARY) {
            named_type = RTTInfo::from<uint8_t>().getStandardName();
        }
    } catch (std::exception &ex) {
        IGNIS_LOG(warning) << "exception: " << ex.what();
    }

    if (named_type.empty()) {
        IGNIS_LOG(warning) << "Incoming partition type not found or is too complex.";
        return type;
    } else {
        IGNIS_LOG(info) << "Partition type found: " << named_type;
    }

    type = executor_data->getType(named_type);
    if (!type) {
        IGNIS_LOG(warning) << "Type '" + named_type +
                              "' is not in the registry, that means  that the type is not compiled in the executor. "
                              "By default only the combinations of the most used types are compiled, the rest are loaded "
                              "from the libraries used by the user.";
    }

    return type;
}
