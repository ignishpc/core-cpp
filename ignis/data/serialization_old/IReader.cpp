
#include "IReader.h"

using namespace std;
using namespace ignis::data::serialization;
using ignis::data::ITypeInfo;
using ignis::data::ITupleBase;
using apache::thrift::protocol::TProtocol;


class ITypeInfoBuilder : public ITypeInfo {
public:

    ITypeInfoBuilder(size_t id, const string &name) : ITypeInfo(id, name) {}

    inline void putArg(ITypeInfo &arg) {
        ITypeInfo::args().push_back(arg);
    }

    inline ITypeInfo &getArg(size_t pos) {
        return ITypeInfo::args()[pos];
    }

    inline static ITypeInfo complete(ITypeInfo torg, ITypeInfo tnew) {
        if (ITypeInfo::voidType() == torg) {
            return tnew;
        }
        for (size_t i = 0; i < torg.types() && i < tnew.types(); i++) {
            ((ITypeInfoBuilder &) torg).getArg(i) = ITypeInfoBuilder::complete(torg[i], tnew[i]);
        }
        return torg;
    }

    inline static void putArg(ITypeInfo &type, ITypeInfo &arg) {
        ((ITypeInfoBuilder &) type).putArg(arg);
    }

    inline static ITypeInfo &getArg(ITypeInfo &type, size_t pos) {
        return ((ITypeInfoBuilder &) type).getArg(pos);
    }
};

template<class T1=Any, class T2=Any>
class ITupleReader : public ITupleBase<T1, T2> {
public:
    ITupleReader() : ITupleBase<T1, T2>(NULL, NULL) {}

    T1 *&pFirst() { return this->first_ptr; }

    T2 *&pSecond() { return this->second_ptr; }

    bool &rHandlePtr() { return this->handle_ptr; }

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
    auto size = readSize(tProtocol);

    auto elem_type_id = readType(tProtocol);
    auto &elem_type_info = getInfo(elem_type_id);
    auto elem_type = ITypeInfoBuilder(elem_type_info.class_id, elem_type_info.class_name);
    auto elem = elem_type_info.handle->newT();

    elem_type_info.handle->resizeVector(obj, size);

    for (decltype(size) i = 0; i < size; i++) {
        (this->*elem_type_info.function)(elem, elem_type, tProtocol);
        elem_type_info.handle->pushVector(obj, elem);
    }

    if (type.types() < 1) {
        ITypeInfoBuilder::putArg(type, elem_type);
    } else {
        ITypeInfoBuilder::getArg(type, 0) = ITypeInfoBuilder::complete(type[0], elem_type);
    }

    elem_type_info.handle->deleteT(elem);
}

size_t IReader::hashList(void *obj, ignis::data::ITypeInfo &type) {
    vector<Any> &v = *(vector<Any> *) obj;
    auto elem_type = type[0];
    auto elem_hash = getInfo(elem_type.getId()).hash;
    size_t hash = 0;
    for (auto it = v.begin(); it != v.end(); it++) {
        hash_combine(hash, (this->*elem_hash)((void *) &(*it), elem_type));
    }
    return hash;
}

void IReader::readSet(void *obj, ITypeInfo &type, TProtocol &tProtocol) {
    auto size = readSize(tProtocol);

    auto elem_type_id = readType(tProtocol);
    auto &elem_type_info = getInfo(elem_type_id);
    auto elem_type = ITypeInfoBuilder(elem_type_info.class_id, elem_type_info.class_name);
    auto elem = elem_type_info.handle->newT();

    elem_type_info.handle->resizeSet(obj, size);

    for (decltype(size) i = 0; i < size; i++) {
        (this->*elem_type_info.function)(elem, elem_type, tProtocol);
        elem_type_info.handle->insertSet(obj, elem, (this->*elem_type_info.hash)(elem, elem_type));
    }

    if (type.types() < 1) {
        ITypeInfoBuilder::putArg(type, elem_type);
    } else {
        ITypeInfoBuilder::getArg(type, 0) = ITypeInfoBuilder::complete(type[0], elem_type);
    }

    elem_type_info.handle->deleteT(elem);
}

size_t IReader::hashSet(void *obj, ignis::data::ITypeInfo &type) {
    unordered_set<Any> &s = *(unordered_set<Any> *) obj;
    auto elem_type = type[0];
    auto elem_hash = getInfo(elem_type.getId()).hash;
    size_t hash = 0;
    for (auto it = s.begin(); it != s.end(); it++) {
        hash_combine(hash, (this->*elem_hash)((void *) &(*it), elem_type));
    }
    return hash;
}

void IReader::readMap(void *obj, ITypeInfo &type, TProtocol &tProtocol) {
    auto size = readSize(tProtocol);

    auto key_type_id = readType(tProtocol);
    auto value_type_id = readType(tProtocol);
    auto &key_type_info = getInfo(key_type_id);
    auto &value_type_info = getInfo(value_type_id);
    auto key_type = ITypeInfoBuilder(key_type_info.class_id, key_type_info.class_name);
    auto value_type = ITypeInfoBuilder(value_type_info.class_id, value_type_info.class_name);
    auto handle = key_type_info.handles[key_type_id];

    auto key = handle->newT();
    auto value = handle->newT2();

    handle->resizeMap(obj, size);

    for (decltype(size) i = 0; i < size; i++) {
        (this->*key_type_info.function)(key, key_type, tProtocol);
        (this->*value_type_info.function)(value, value_type, tProtocol);
        handle->putMap(obj, key, value, (this->*key_type_info.hash)(key, key_type));
    }

    if (type.types() < 2) {
        ITypeInfoBuilder::putArg(type, key_type);
        ITypeInfoBuilder::putArg(type, value_type);
    } else {
        ITypeInfoBuilder::getArg(type, 0) = ITypeInfoBuilder::complete(type[0], key_type);
        ITypeInfoBuilder::getArg(type, 1) = ITypeInfoBuilder::complete(type[1], value_type);
    }

    handle->deleteT(key);
    handle->deleteT2(value);
}

size_t IReader::hashMap(void *obj, ignis::data::ITypeInfo &type) {
    unordered_map<Any, Any> &m = *(unordered_map<Any, Any> *) obj;
    auto key_type = type[0];
    auto value_type = type[1];
    auto &key_hash = getInfo(key_type.getId()).hash;
    auto &value_hash = getInfo(value_type.getId()).hash;
    size_t hash = 0;
    for (auto it = m.begin(); it != m.end(); it++) {
        hash_combine(hash, (this->*key_hash)((void *) &(it->first), key_type));
        hash_combine(hash, (this->*value_hash)((void *) &(it->second), value_type));
    }
    return hash;
}

void IReader::readBinary(void *obj, ITypeInfo &type, TProtocol &tProtocol) {
    auto size = readSize(tProtocol);

    auto &elem_type_info = getInfo(IEnumTypes::I_I08);
    auto elem_type = ITypeInfoBuilder(elem_type_info.class_id, elem_type_info.class_name);

    vector<__int8_t> &binary = *(vector<__int8_t> *) obj;
    binary.resize(size, 0);

    for (decltype(size) i = 0; i < size; i++) {
        tProtocol.readByte(binary[0]);
    }

    if (type.types() < 1) {
        ITypeInfoBuilder::putArg(type, elem_type);
    }
}

void IReader::readTuple(void *obj, ITypeInfo &type, TProtocol &tProtocol) {
    auto first_type_id = readType(tProtocol);
    auto second_type_id = readType(tProtocol);
    auto &first_type_info = getInfo(first_type_id);
    auto &second_type_info = getInfo(second_type_id);
    auto first_type = ITypeInfoBuilder(first_type_info.class_id, first_type_info.class_name);
    auto second_type = ITypeInfoBuilder(second_type_info.class_id, second_type_info.class_name);
    auto handle = first_type_info.handles[second_type_id];

    ((ITupleReader<> *) obj)->pFirst() = (Any *) handle->newT();
    ((ITupleReader<> *) obj)->pSecond() = (Any *) handle->newT2();

    (this->*first_type_info.function)(handle->firstPair(obj), first_type, tProtocol);
    (this->*second_type_info.function)(handle->secondPair(obj), second_type, tProtocol);

    if (type.types() < 2) {
        ITypeInfoBuilder::putArg(type, first_type);
        ITypeInfoBuilder::putArg(type, second_type);
    } else {
        ITypeInfoBuilder::getArg(type, 0) = ITypeInfoBuilder::complete(type[0], first_type);
        ITypeInfoBuilder::getArg(type, 1) = ITypeInfoBuilder::complete(type[1], second_type);
    }
}

size_t IReader::hashTuple(void *obj, ignis::data::ITypeInfo &type) {
    ITuple <Any, Any> &t = *(ITuple <Any, Any> *) obj;
    auto first_type = type[0];
    auto second_type = type[1];
    auto &first_hash = getInfo(first_type.getId()).hash;
    auto &second_hash = getInfo(second_type.getId()).hash;
    size_t hash = 0;

    hash_combine(hash, (this->*first_hash)((void *) &(t.first()), first_type));
    hash_combine(hash, (this->*second_hash)((void *) &(t.second()), second_type));

    return hash;
}

inline __uint8_t IReader::readType(apache::thrift::protocol::TProtocol &tProtocol) {
    __int8_t type;
    tProtocol.readByte(type);
    return type;
}

inline size_t IReader::readSize(apache::thrift::protocol::TProtocol &tProtocol) {
    __int64_t size;
    tProtocol.readI64(size);
    return size;
}

IReader::IReader() {

    newMultipleType<bool>(IEnumTypes::I_VOID, &IReader::readVoid);
    newMultipleType<bool>(IEnumTypes::I_BOOL, &IReader::readBool);
    newMultipleType<__int8_t>(IEnumTypes::I_I08, &IReader::readI08);
    newMultipleType<__int16_t>(IEnumTypes::I_I16, &IReader::readI16);
    newMultipleType<__int32_t>(IEnumTypes::I_I32, &IReader::readI32);
    newMultipleType<__int64_t>(IEnumTypes::I_I64, &IReader::readI64);
    newMultipleType<double>(IEnumTypes::I_DOUBLE, &IReader::readDouble);
    newMultipleType<string>(IEnumTypes::I_STRING, &IReader::readString);
    newMultipleType<vector<Any>>(IEnumTypes::I_LIST, &IReader::readList, &IReader::hashList);
    newMultipleType<unordered_set<Any>>(IEnumTypes::I_SET, &IReader::readSet, &IReader::hashSet);
    newMultipleType<unordered_map<Any, Any>>(IEnumTypes::I_MAP, &IReader::readMap, &IReader::hashMap);
    newMultipleType<ITupleBase<Any, Any>>(IEnumTypes::I_TUPLE, &IReader::readTuple, &IReader::hashTuple);
    newMultipleType<vector<Any>>(IEnumTypes::I_BINARY, &IReader::readBinary);

    updateInfo();

    auto ivoid = *(IReader::IReaderInfo *) &getInfo(IEnumTypes::I_VOID);
    ivoid.class_id = ITypeInfo::voidType().getId();
    ivoid.class_name = ITypeInfo::voidType().nameId();
}

std::pair<std::shared_ptr<void>, ITypeInfo> IReader::readObject(apache::thrift::protocol::TProtocol &tProtocol) {
    auto elem_type_id = readType(tProtocol);
    auto &elem_type_info = getInfo(elem_type_id);
    auto elem_type = ITypeInfoBuilder(elem_type_info.class_id, elem_type_info.class_name);
    auto result = pair<std::shared_ptr<void>, ITypeInfo>(shared_ptr<Any>(), ITypeInfo::voidType());

    result.first = shared_ptr<Any>((Any *) elem_type_info.handle->newT());
    (this->*elem_type_info.function)(&result.second, elem_type, tProtocol);
    return result;
}

inline IReader::IReaderInfo &IReader::getInfo(__uint8_t id) {
    auto entry = info_map.find(id);
    if (entry == info_map.end()) {
        //No exists exception
    }
    return entry->second;
}

void IReader::updateInfo() {
    for (auto it = info_map.begin(); it != info_map.end(); it++) {
        if (it->second.multiple_type) {
            for (auto it2 = info_map.begin(); it2 != info_map.end(); it2++) {
                info_map[it->first].handles[it2->first] =
                        it->second.container->newInstance(it2->second.container.get());
            }
            info_map[it->first].handle = info_map[it->first].handles.begin()->second;
        } else {
            info_map[it->first].handle = it->second.container->newInstance();
        }
    }
}


