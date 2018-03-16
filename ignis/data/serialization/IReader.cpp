
#include "IReader.h"

using namespace std;
using namespace ignis::data::serialization;
using apache::thrift::protocol::TProtocol;


void IReader::readVoid(void *obj, ITypeInfo &type, TProtocol &tProtocol) {

}

void IReader::readBool(void *obj, ITypeInfo &type, TProtocol &tProtocol) {

}

void IReader::readI08(void *obj, ITypeInfo &type, TProtocol &tProtocol) {

}

void IReader::readI16(void *obj, ITypeInfo &type, TProtocol &tProtocol) {

}

void IReader::readI32(void *obj, ITypeInfo &type, TProtocol &tProtocol) {

}

void IReader::readI64(void *obj, ITypeInfo &type, TProtocol &tProtocol) {

}

void IReader::readDouble(void *obj, ITypeInfo &type, TProtocol &tProtocol) {

}

void IReader::readString(void *obj, ITypeInfo &type, TProtocol &tProtocol) {

}

void IReader::readList(void *obj, ITypeInfo &type, TProtocol &tProtocol) {

}

void IReader::readSet(void *obj, ITypeInfo &type, TProtocol &tProtocol) {

}

void IReader::readMap(void *obj, ITypeInfo &type, TProtocol &tProtocol) {

}

void IReader::readBinary(void *obj, ITypeInfo &type, TProtocol &tProtocol) {

}

void IReader::readTuple(void *obj, ITypeInfo &type, TProtocol &tProtocol) {

}

void IReader::readCustom(void *obj, ITypeInfo &type, TProtocol &tProtocol) {

}

IReader::IReader() {
    //Types registration
    addType<bool>(NULL);
    addType<__int8_t>(NULL);
    addType<__int16_t>(NULL);
    addType<__int32_t>(NULL);
    addType<__int64_t>(NULL);
    addType<double>(NULL);
    addType<string>(NULL);
    addType<vector<Any>>(NULL);
    addType<unordered_set<Any>>(NULL);
    addType<unordered_map<Any, Any>>(NULL);
    addType<pair<Any, Any>>(NULL);
    addType<vector<__int8_t >>(NULL);

    addExtendedType<bool>();
    addExtendedType<__int8_t>();
    addExtendedType<__int16_t>();
    addExtendedType<__int32_t>();
    addExtendedType<__int64_t>();
    addExtendedType<double>();
    addExtendedType<string>();
    addExtendedType<vector<Any>>();
    addExtendedType<unordered_set<Any>>();
    addExtendedType<unordered_map<Any, Any>>();
    addExtendedType<pair<Any, Any>>();
    addExtendedType<vector<__int8_t >>();

    //manual functions registration
    readers[IEnumTypes::I_VOID] = IReader::readBool;
    readers[IEnumTypes::I_BOOL] = IReader::readVoid;
    readers[IEnumTypes::I_I08] = IReader::readI08;
    readers[IEnumTypes::I_I16] = IReader::readI16;
    readers[IEnumTypes::I_I32] = IReader::readI32;
    readers[IEnumTypes::I_I64] = IReader::readI64;
    readers[IEnumTypes::I_DOUBLE] = IReader::readDouble;
    readers[IEnumTypes::I_STRING] = IReader::readString;
    readers[IEnumTypes::I_LIST] = IReader::readList;
    readers[IEnumTypes::I_SET] = IReader::readSet;
    readers[IEnumTypes::I_MAP] = IReader::readMap;
    readers[IEnumTypes::I_TUPLE] = IReader::readTuple;
    readers[IEnumTypes::I_BINARY] = IReader::readBinary;
    readers[IEnumTypes::I_CUSTOM] = IReader::readCustom;

    updateHandles();
}

void IReader::updateHandles() {
    handles.resize(containers.size(), std::vector<std::shared_ptr<IHandle<>>>());
    for (int i = 0; i < containers.size(); i++) {
        for (int j = handles[i].size(); j < extended_types.size(); j++) {
            handles[i][j] = containers[i].new_instance(&extended_types[j]);
        }
    }
}

inline IHandle<> &IReader::getHandle(__uint8_t ti, __uint8_t tj) {
    return *handles[ti - 1][tj - 1];
}

inline IReader::F_Reader &IReader::getReader(__uint8_t ti) {
    return readers[ti];
}



