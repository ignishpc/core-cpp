
#include "IReader.h"

using namespace std;
using namespace ignis::data::serialization;
using ignis::data::ITypeInfo;
using apache::thrift::protocol::TProtocol;


class ITypeInfoBuilder : public ITypeInfo {
public:

    ITypeInfoBuilder(size_t id, string &name) : ITypeInfo(id, name) {}

    void putArg(ITypeInfo &arg) {
        ITypeInfo::putArg(arg);
    }

    static void putArg(ITypeInfo &type, ITypeInfo &arg) {
        ((ITypeInfoBuilder &) type).putArg(arg);
    }
};

void IReader::readVoid(void *obj, ITypeInfo &type, TProtocol &tProtocol) {
    //Nothing
}

void IReader::readBool(void *obj, ITypeInfo &type, TProtocol &tProtocol) {
    tProtocol.readBool(*(bool *) obj);
}

void IReader::readI08(void *obj, ITypeInfo &type, TProtocol &tProtocol) {
    tProtocol.readByte(*(int8_t *) obj);
}

void IReader::readI16(void *obj, ITypeInfo &type, TProtocol &tProtocol) {
    tProtocol.readI16(*(int16_t *) obj);
}

void IReader::readI32(void *obj, ITypeInfo &type, TProtocol &tProtocol) {
    tProtocol.readI32(*(int32_t *) obj);
}

void IReader::readI64(void *obj, ITypeInfo &type, TProtocol &tProtocol) {
    tProtocol.readI64(*(int64_t *) obj);
}

void IReader::readDouble(void *obj, ITypeInfo &type, TProtocol &tProtocol) {
    tProtocol.readDouble(*(double *) obj);
}

void IReader::readString(void *obj, ITypeInfo &type, TProtocol &tProtocol) {
    tProtocol.readString(*(string *) obj);
}

void IReader::readList(void *obj, ITypeInfo &type, TProtocol &tProtocol) {
    auto type_size = readSize(tProtocol);
    auto type_id = readType(tProtocol);
    auto type_info = getInfo(type_id);
    auto elem = type_info.handle->newT();
    auto elem_type = ITypeInfoBuilder(type_info.class_id, type_info.class_name);

    type_info.handle->resizeVector(obj, type_size);

    for (decltype(type_size) i = 0; i < type_size; i++) {
        (this->*type_info.function)(elem, elem_type, tProtocol);
        type_info.handle->pushVector(obj, elem);
    }

    ITypeInfoBuilder::putArg(type, elem_type);

    type_info.handle->deleteT(elem);
}

void IReader::readSet(void *obj, ITypeInfo &type, TProtocol &tProtocol) {
    auto type_size = readSize(tProtocol);
    auto type_id = readType(tProtocol);
    auto type_info = getInfo(type_id);
    auto elem = type_info.handle->newT();
    auto elem_type = ITypeInfoBuilder(type_info.class_id, type_info.class_name);

    type_info.handle->resizeSet(obj, type_size);

    //TODO

    ITypeInfoBuilder::putArg(type, elem_type);

    type_info.handle->deleteT(elem);
}

void IReader::readMap(void *obj, ITypeInfo &type, TProtocol &tProtocol) {

}

void IReader::readBinary(void *obj, ITypeInfo &type, TProtocol &tProtocol) {

}

void IReader::readTuple(void *obj, ITypeInfo &type, TProtocol &tProtocol) {

}

inline __uint8_t IReader::readType(apache::thrift::protocol::TProtocol &tProtocol) {
    __int8_t type;
    tProtocol.readByte(type);
    auto entry = info_map.find(type);
    if (entry == info_map.end()) {
        //No exists exception
    }
    return type;
}

inline size_t IReader::readSize(apache::thrift::protocol::TProtocol &tProtocol) {
    __int64_t size;
    tProtocol.readI64(size);
    return size;
}

IReader::IReader() {

    F_Reader a;

    newExtendedType<bool>(IEnumTypes::I_VOID, &IReader::readBool);
    newExtendedType<bool>(IEnumTypes::I_BOOL, &IReader::readBool);
    newExtendedType<__int8_t>(IEnumTypes::I_I08, &IReader::readI08);
    newExtendedType<__int16_t>(IEnumTypes::I_I16, &IReader::readI16);
    newExtendedType<__int32_t>(IEnumTypes::I_I32, &IReader::readI32);
    newExtendedType<__int64_t>(IEnumTypes::I_I64, &IReader::readI64);
    newExtendedType<double>(IEnumTypes::I_DOUBLE, &IReader::readDouble);
    newExtendedType<string>(IEnumTypes::I_STRING, &IReader::readString);
    newExtendedType<vector<Any>>(IEnumTypes::I_LIST, &IReader::readList);
    newExtendedType<unordered_set<Any>>(IEnumTypes::I_SET, &IReader::readSet);
    newExtendedType<unordered_map<Any, Any>>(IEnumTypes::I_MAP, &IReader::readMap);
    newExtendedType<pair<Any, Any>>(IEnumTypes::I_TUPLE, &IReader::readTuple);
    newExtendedType<vector<Any>>(IEnumTypes::I_BINARY, &IReader::readBinary);

    updateInfo();
}


inline const IReader::IReaderInfo &IReader::getInfo(__uint8_t id) {
    return info_map[id];
}

void IReader::updateInfo() {
    for (auto it = info_map.begin(); it != info_map.end(); it++) {
        if (it->second.extended_type) {
            for (auto it2 = info_map.begin(); it2 != info_map.end(); it2++) {
                info_map[it2->first].handles[it->first] =
                        it2->second.container->new_instance(it->second.extended_type.get());
            }
        }
    }
}


