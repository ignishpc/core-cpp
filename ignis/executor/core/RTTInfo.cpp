
#include "RTTInfo.h"
#include "cxxabi.h"

using namespace ignis::executor::core;

RTTInfo::RTTInfo(const std::type_info &info) : info(info) {}

const std::type_info &RTTInfo::getInfo() const {
    return info;
}

std::string RTTInfo::getStandardName() const {
    int status;
    char *name = abi::__cxa_demangle(this->info.name(), 0, 0, &status);
    if (status == 0) {
        std::string result(name);
        free(name);
        return result;
    }
    return std::string();
}

bool RTTInfo::isPrimitive() const {
    return typeid(abi::__fundamental_type_info) == typeid(this->info);
}

bool RTTInfo::isArray() const {
    return typeid(abi::__array_type_info) == typeid(this->info);
}

bool RTTInfo::isFunction() const {
    return typeid(abi::__function_type_info) == typeid(this->info);
}

bool RTTInfo::isEnum() const {
    return typeid(abi::__enum_type_info) == typeid(this->info);
}

bool RTTInfo::isClass() const {
    return typeid(abi::__class_type_info) == typeid(this->info);
}

bool RTTInfo::isSimpleClass() const {
    return typeid(abi::__si_class_type_info) == typeid(this->info);
}

bool RTTInfo::isVirtalClass() const {
    return typeid(abi::__vmi_class_type_info) == typeid(this->info);
}

bool RTTInfo::isBaseOf(const std::type_info &info) const {
    return RTTInfo(info).isDerivedOf(this->info);
}

bool RTTInfo::isDerivedOf(const std::type_info &info) const {
    if (isVirtalClass()) {
        for (int i = 0; i < getNumOfBases(); i++) {
            if (this->info == info || RTTInfo(getBase(i)).isDerivedOf(info)) {
                return true;
            }
        }
    } else if (isSimpleClass()) {
        return this->info == info || RTTInfo(getBase()).isDerivedOf(info);
    }
    return this->info == info;
}

unsigned int RTTInfo::getNumOfBases() const {
    if (isVirtalClass()) {
        const abi::__vmi_class_type_info *class_info = (const abi::__vmi_class_type_info *) &this->info;
        return class_info->__base_count;
    } else if (isSimpleClass()) {
        return 1;
    } else {
        return 0;
    }
}

const std::type_info &RTTInfo::getBase(int p) const {
    if (isVirtalClass()) {
        const abi::__vmi_class_type_info *class_info = (const abi::__vmi_class_type_info *) &this->info;
        return (std::type_info &) *class_info->__base_info[p].__base_type;
    } else if (isSimpleClass() && p == 0) {
        const abi::__si_class_type_info *class_info = (const abi::__si_class_type_info *) &this->info;
        return (std::type_info &) *class_info->__base_type;
    }
}

const std::type_info &RTTInfo::getBase() const {
    return getBase(0);
}

bool RTTInfo::isVoid() const {
    return typeid(void) == this->info;
}

bool RTTInfo::isPointer() const {
    return typeid(abi::__pointer_type_info) == typeid(this->info);
}

const std::type_info &RTTInfo::getClassPointed() const {
    const abi::__pointer_type_info *class_info = (const abi::__pointer_type_info *) &this->info;
    return (std::type_info &) *class_info->__pointee;
}

bool RTTInfo::isMemberPointer() const {
    return typeid(abi::__pointer_to_member_type_info) == typeid(this->info);
}

const std::type_info &RTTInfo::getClassMemberPointed() const {
    const abi::__pointer_to_member_type_info *class_info = (const abi::__pointer_to_member_type_info *) &this->info;
    return (std::type_info &) *class_info->__context;
}

bool ignis::executor::core::operator==(const RTTInfo &lhs, const RTTInfo &rhs) {
    return lhs.info == rhs.info;
}

bool ignis::executor::core::operator!=(const RTTInfo &lhs, const RTTInfo &rhs) {
    return !(rhs == lhs);
}