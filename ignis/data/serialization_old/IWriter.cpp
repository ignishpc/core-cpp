
#include "IWriter.h"

using namespace std;
using namespace ignis::data::serialization;
using ignis::data::ITypeInfo;
using apache::thrift::protocol::TProtocol;

inline void IWriter::writeBool(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol) {
    tProtocol.writeBool(*(bool *) obj);
}

inline void IWriter::writeI08(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol) {
    tProtocol.writeByte(*(__int8_t *) obj);
}

inline void IWriter::writeI16(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol) {
    tProtocol.writeI16(*(__int16_t *) obj);
}

inline void IWriter::writeI32(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol) {
    tProtocol.writeI32(*(__int32_t *) obj);
}

inline void IWriter::writeI64(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol) {
    tProtocol.writeI64(*(__int64_t *) obj);
}

inline void IWriter::writeDouble(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol) {
    tProtocol.writeDouble(*(double *) obj);
}

inline void IWriter::writeString(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol) {
    tProtocol.writeString(*(string *) obj);
}

inline void IWriter::writeList(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol) {
    vector<Any> &v = *(vector<Any> *) obj;
    auto elem_type = type[0];
    auto elem_type_info = getInfo(elem_type.getId());

    size_t elem_size = (this->*getInfo(elem_type.getId()).size)(elem_type);
    size_t size = v.size() / elem_size;
    auto data = v.data();

    writeSize(size, tProtocol);
    if (elem_type_info.id != IEnumTypes::I_I08) {
        writeType(elem_type_info.id, elem_type, tProtocol);
    }
    for (size_t i = 0; i < size; i++) {
        (this->*elem_type_info.function)(data + i * elem_size, elem_type, tProtocol);
    }
}

inline void IWriter::writeTypeList(__uint8_t id, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol) {
    auto elem_type = type[0];
    if ((ITypeInfo::getInfo<__int8_t>() == elem_type) || (ITypeInfo::getInfo<__uint8_t>() == elem_type)) {
        writeType(IEnumTypes::I_BINARY, elem_type, tProtocol);
    }
}

inline void IWriter::writeSet(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol) {
    unordered_set<Any> &s = *(unordered_set<Any> *) obj;
    auto elem_type = type[0];
    auto elem_type_info = getInfo(elem_type.getId());

    writeSize(s.size(), tProtocol);
    writeType(elem_type_info.id, elem_type, tProtocol);
    for (auto it = s.begin(); it != s.end(); it++) {
        (this->*elem_type_info.function)((void *) &(*it), elem_type, tProtocol);
    }
}

inline void IWriter::writeMap(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol) {
    unordered_map<Any, Any> &m = *(unordered_map<Any, Any> *) obj;
    auto key_type = type[0];
    auto value_type = type[1];
    auto key_type_info = getInfo(key_type.getId());
    auto value_type_info = getInfo(value_type.getId());

    writeSize(m.size(), tProtocol);
    writeType(key_type_info.id, key_type, tProtocol);
    writeType(value_type_info.id, value_type, tProtocol);
    for (auto it = m.begin(); it != m.end(); it++) {
        (this->*key_type_info.function)((void *) &(it->first), key_type, tProtocol);
        (this->*value_type_info.function)((void *) &(it->second), value_type, tProtocol);
    }
}

inline void IWriter::writeTuple(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol) {
    pair<Any, Any> &t = *(pair<Any, Any> *) obj;
    auto first_type = type[0];
    auto second_type = type[1];
    auto first_type_info = getInfo(first_type.getId());
    auto second_type_info = getInfo(second_type.getId());
    writeType(first_type_info.id, first_type, tProtocol);
    writeType(second_type_info.id, second_type, tProtocol);
    (this->*first_type_info.function)((void *) &(t.first), first_type, tProtocol);
    (this->*second_type_info.function)((void *) (&(t.first) + (this->*getInfo(first_type.getId()).size)(first_type)), second_type, tProtocol);
}

inline size_t IWriter::getSizeTuple(ITypeInfo &type) {
    auto first_type = type[0];
    auto second_type = type[1];
    return (this->*getInfo(first_type.getId()).size)(first_type) +
           (this->*getInfo(second_type.getId()).size)(second_type);
}

void IWriter::writeUI08(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol) {
    int16_t c_obj = *(uint8_t *) obj;
    writeI16(&obj, type, tProtocol);
}

void IWriter::writeUI16(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol) {
    int32_t c_obj = *(uint16_t *) obj;
    writeI32(&obj, type, tProtocol);
}

void IWriter::writeUI32(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol) {
    int64_t c_obj = *(uint32_t *) obj;
    writeI64(&obj, type, tProtocol);
}

void IWriter::writeUI64(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol) {
    int64_t c_obj = *(uint64_t *) obj;
    writeI64(&obj, type, tProtocol);
}

void IWriter::writeFloat(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol) {
    double c_obj = *(float *) obj;
    writeDouble(&obj, type, tProtocol);
}

void IWriter::writeCppList(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol) {
    list<Any> &l = *(list<Any> *) obj;
    auto elem_type = type[0];
    auto elem_type_info = getInfo(elem_type.getId());

    writeSize(l.size(), tProtocol);
    writeType(elem_type_info.id, elem_type, tProtocol);
    for (auto it = l.begin(); it != l.end(); it++) {
        (this->*elem_type_info.function)((void *) &(*it), elem_type, tProtocol);
    }
}

void IWriter::writeForwardList(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol) {
    forward_list<Any> &l = *(forward_list<Any> *) obj;
    auto elem_type = type[0];
    auto elem_type_info = getInfo(elem_type.getId());
    size_t list_size = 0;
    for (auto it = l.begin(); it != l.end(); it++) {
        list_size++;
    }

    writeSize(list_size, tProtocol);
    writeType(elem_type_info.id, elem_type, tProtocol);
    for (auto it = l.begin(); it != l.end(); it++) {
        (this->*elem_type_info.function)((void *) &(*it), elem_type, tProtocol);
    }
}

void IWriter::writeTreeSet(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol) {
    unordered_set<Any> &s = *(unordered_set<Any> *) obj;
    auto elem_type = type[0];
    auto elem_type_info = getInfo(elem_type.getId());

    writeSize(s.size(), tProtocol);
    writeType(elem_type_info.id, elem_type, tProtocol);
    for (auto it = s.begin(); it != s.end(); it++) {
        (this->*elem_type_info.function)((void *) &(*it), elem_type, tProtocol);
    }
}

void IWriter::writeTreeMap(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol) {
    map<Any, Any> &m = *(map<Any, Any> *) obj;
    auto key_type = type[0];
    auto value_type = type[1];
    auto key_type_info = getInfo(key_type.getId());
    auto value_type_info = getInfo(value_type.getId());

    writeSize(m.size(), tProtocol);
    writeType(key_type_info.id, key_type, tProtocol);
    writeType(value_type_info.id, value_type, tProtocol);
    for (auto it = m.begin(); it != m.end(); it++) {
        (this->*key_type_info.function)((void *) &(it->first), key_type, tProtocol);
        (this->*value_type_info.function)((void *) &(it->second), value_type, tProtocol);
    }
}

void IWriter::writePointer(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol) {
    void *p = *(void **) obj;
    auto elem_type = type[0];
    auto elem_type_info = getInfo(elem_type.getId());
    (this->*elem_type_info.function)(p, elem_type, tProtocol);
}

void IWriter::writeSharedPointer(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol) {
    shared_ptr<void> &sp = *(shared_ptr<void> *) obj;
    auto elem_type = type[0];
    auto elem_type_info = getInfo(elem_type.getId());
    (this->*elem_type_info.function)(sp.get(), elem_type, tProtocol);
}

void IWriter::writeType(__uint8_t id, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol) {
    tProtocol.writeByte(id);
}

inline void IWriter::writeSize(size_t size, apache::thrift::protocol::TProtocol &tProtocol) {
    tProtocol.writeI64(size);
}

IWriter::IWriter() {

    newType<bool>(IEnumTypes::I_BOOL, &IWriter::writeBool);
    newType<__int8_t>(IEnumTypes::I_I08, &IWriter::writeI08);
    newType<__int16_t>(IEnumTypes::I_I16, &IWriter::writeI16);
    newType<__int32_t>(IEnumTypes::I_I32, &IWriter::writeI32);
    newType<__int64_t>(IEnumTypes::I_I64, &IWriter::writeI64);
    newType<double>(IEnumTypes::I_DOUBLE, &IWriter::writeDouble);
    newType<string>(IEnumTypes::I_STRING, &IWriter::writeString);
    newType<vector<Any>>(IEnumTypes::I_LIST, &IWriter::writeList, &IWriter::writeTypeList);
    newType<unordered_set<Any>>(IEnumTypes::I_SET, &IWriter::writeSet);
    newType<unordered_map<Any, Any>>(IEnumTypes::I_MAP, &IWriter::writeMap);
    newType<pair<Any, Any>>(IEnumTypes::I_TUPLE, &IWriter::writeTuple, &IWriter::writeType, &IWriter::getSizeTuple);
    //Other C++ types
    newType<__uint8_t>(IEnumTypes::I_I16, &IWriter::writeUI08);
    newType<__uint16_t>(IEnumTypes::I_I32, &IWriter::writeUI16);
    newType<__uint32_t>(IEnumTypes::I_I64, &IWriter::writeUI32);
    newType<__uint64_t>(IEnumTypes::I_I64, &IWriter::writeUI64);
    newType<float>(IEnumTypes::I_I64, &IWriter::writeFloat);
    newType<list<Any>>(IEnumTypes::I_LIST, &IWriter::writeCppList);
    newType<forward_list<Any>>(IEnumTypes::I_LIST, &IWriter::writeForwardList);
    newType<set<Any>>(IEnumTypes::I_LIST, &IWriter::writeTreeSet);
    newType<map<Any, Any>>(IEnumTypes::I_LIST, &IWriter::writeTreeMap);
    newType<Any *>(IEnumTypes::I_LIST, &IWriter::writePointer);
    newType<shared_ptr<Any>>(IEnumTypes::I_LIST, &IWriter::writeSharedPointer);

}

void IWriter::writeObject(void *obj, ignis::data::ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol) {
    auto elem_type_info = getInfo(type.getId());
    writeType(elem_type_info.id, type, tProtocol);
    (this->*elem_type_info.function)(obj, type, tProtocol);
}

inline const IWriter::IWriterInfo &IWriter::getInfo(size_t id) {
    auto entry = info_map.find(id);
    if (entry == info_map.end()) {
        //No exists exceptions
    }
    return entry->second;
}

