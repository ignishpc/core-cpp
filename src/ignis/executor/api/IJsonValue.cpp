
#include "IJsonValue.h"
#include "ignis/executor/core/io/IEnumTypes.h"

using namespace ignis::executor::api;
using namespace ignis::executor::core::io;
using ignis::executor::core::io::IEnumTypes;

bool IJsonValue::isNull() const {
    return type == IEnumTypes::I_VOID;
}

bool IJsonValue::isBoolean() const {
    return type == IEnumTypes::I_BOOL;
}

bool IJsonValue::isInteger() const {
    return type == IEnumTypes::I_I64;
}

bool IJsonValue::isDouble() const {
    return type == IEnumTypes::I_DOUBLE;
}

bool IJsonValue::isNumber() const {
    return type == IEnumTypes::I_DOUBLE || type == IEnumTypes::I_I64;
}

bool IJsonValue::isString() const {
    return type == IEnumTypes::I_STRING;
}

bool IJsonValue::isArray() const {
    return type == IEnumTypes::I_LIST;
}

bool IJsonValue::isMap() const {
    return type == IEnumTypes::I_MAP;
}

IJsonValue::IJsonValue() {
    setNull();
}

void IJsonValue::setNull() {
    type = IEnumTypes::I_VOID;
}

IJsonValue::IJsonValue(bool value) {
    setBoolean(value);
}

void IJsonValue::setBoolean(bool value) {
    type = IEnumTypes::I_BOOL;
    elem = std::make_shared<bool>(value);
}

bool IJsonValue::getBoolean() const {
    return *reinterpret_cast<bool *>(elem.get());
}

IJsonValue::IJsonValue(int64_t value) {
    setInteger(value);
}

void IJsonValue::setInteger(int64_t value) {
    type = IEnumTypes::I_I64;
    elem = std::make_shared<int64_t>(value);
}

int64_t IJsonValue::getInteger() const {
    return *reinterpret_cast<int64_t *>(elem.get());
}

double IJsonValue::getDouble() const {
    return *reinterpret_cast<double *>(elem.get());
}

IJsonValue::IJsonValue(double value) {
    setDouble(value);
}

void IJsonValue::setDouble(double value) {
    type = IEnumTypes::I_DOUBLE;
    elem = std::make_shared<double>(value);
}

double IJsonValue::getNumber() const {
    if (isDouble()) {
        return getDouble();
    } else {
        return (double) getInteger();
    }
}

IJsonValue::IJsonValue(const std::string &value) {
    setString(value);
}

IJsonValue::IJsonValue(std::string &&value) {
    setString(std::forward<std::string>(value));
}

void IJsonValue::setString(const std::string &value) {
    type = IEnumTypes::I_STRING;
    elem = std::make_shared<std::string>(value);
}

void IJsonValue::setString(std::string &&value) {
    type = IEnumTypes::I_STRING;
    elem = std::make_shared<std::string>(value);
}

const std::string &IJsonValue::getString() const {
    return *reinterpret_cast<const std::string *>(elem.get());
}

IJsonValue::IJsonValue(const std::vector<IJsonValue> &value) {
    setArray(value);
}

IJsonValue::IJsonValue(std::vector<IJsonValue> &&value) {
    setArray(std::forward<std::vector<IJsonValue>>(value));
}

void IJsonValue::setArray(const std::vector<IJsonValue> &value) {
    type = IEnumTypes::I_LIST;
    elem = std::make_shared<std::vector<IJsonValue>>(value);
}

void IJsonValue::setArray(std::vector<IJsonValue> &&value) {
    type = IEnumTypes::I_LIST;
    elem = std::make_shared<std::vector<IJsonValue>>(value);
}

const std::vector<IJsonValue> &IJsonValue::getArray() const {
    return *reinterpret_cast<const std::vector<IJsonValue> *>(elem.get());
}

IJsonValue::IJsonValue(const std::unordered_map<std::string, IJsonValue> &value) {
    setMap(value);
}

IJsonValue::IJsonValue(std::unordered_map<std::string, IJsonValue> &&value) {
    setMap(std::forward<std::unordered_map<std::string, IJsonValue>>(value));
}

void IJsonValue::setMap(const std::unordered_map<std::string, IJsonValue> &value) {
    type = IEnumTypes::I_MAP;
    elem = std::make_shared<std::unordered_map<std::string, IJsonValue>>(value);
}

void IJsonValue::setMap(std::unordered_map<std::string, IJsonValue> &&value) {
    type = IEnumTypes::I_MAP;
    elem = std::make_shared<std::unordered_map<std::string, IJsonValue>>(value);
}

const std::unordered_map<std::string, IJsonValue> &IJsonValue::getMap() const {
    return *reinterpret_cast<const std::unordered_map<std::string, IJsonValue> *>(elem.get());
}

int IJsonValue::getTypeId() const {
    return type;
}

void (*IJsonWriterType<IJsonValue>::writers[])(JsonWriter &out, const api::IJsonValue &v) = {
        [](JsonWriter &out, const api::IJsonValue &v) {//I_VOID = 0x0
            out.Null();
        },
        [](JsonWriter &out, const api::IJsonValue &v) {//I_BOOL = 0x1
            IJsonWriterType<bool>()(out, v.getBoolean());
        },
        nullptr,//I_I08 = 0x2,
        nullptr,//I_I16 = 0x3,
        nullptr,//I_I32 = 0x4,
        [](JsonWriter &out, const api::IJsonValue &v) {//I_I64 = 0x5
            IJsonWriterType<int64_t>()(out, v.getInteger());
        },
        [](JsonWriter &out, const api::IJsonValue &v) {//I_DOUBLE = 0x6
            IJsonWriterType<double>()(out, v.getDouble());
        },
        [](JsonWriter &out, const api::IJsonValue &v) {//I_STRING = 0x7
            IJsonWriterType<std::string>()(out, v.getString());
        },
        [](JsonWriter &out, const api::IJsonValue &v) {//I_LIST = 0x8
            IJsonWriterType<std::vector<api::IJsonValue>>()(out, v.getArray());
        },
        nullptr,//I_SET = 0x9,
        [](JsonWriter &out, const api::IJsonValue &v) {//I_MAP = 0xa
            IJsonWriterType<std::unordered_map<std::string, api::IJsonValue>>()(out, v.getMap());
        },
        nullptr,//I_PAIR = 0xb,
        nullptr,//I_BINARY = 0xc,
        nullptr,//I_PAIR_LIST = 0xd,
        nullptr,//I_JSON = 0xe,
};

void (*IJsonReaderType<IJsonValue>::readers[])(JsonNode &in, api::IJsonValue &v){
        [](JsonNode &in, api::IJsonValue &v) {//kNullType = 0
            v.setNull();
        },
        [](JsonNode &in, api::IJsonValue &v) {//kFalseType = 1,
            v.setBoolean(false);;
        },
        [](JsonNode &in, api::IJsonValue &v) {//kTrueType = 2,
            v.setBoolean(true);;
        },
        [](JsonNode &in, api::IJsonValue &v) {//kObjectType = 3
            v.setMap(IJsonReaderType<std::unordered_map<std::string, api::IJsonValue>>()(in));
        },
        [](JsonNode &in, api::IJsonValue &v) {//kArrayType = 4
            v.setArray(IJsonReaderType<std::vector<api::IJsonValue>>()(in));
        },
        [](JsonNode &in, api::IJsonValue &v) {//kStringType = 5
            v.setString(IJsonReaderType<std::string>()(in));
        },
        [](JsonNode &in, api::IJsonValue &v) {//kNumberType = 6
            if (in.IsDouble()) {
                v.setDouble(IJsonReaderType<double>()(in));
            } else {
                v.setInteger(IJsonReaderType<int64_t>()(in));
            }
        }
};

void (*IWriterType<IJsonValue>::writers[])(protocol::IProtocol &protocol, const api::IJsonValue &obj){
        [](protocol::IProtocol &protocol, const api::IJsonValue &obj) {//I_VOID = 0x0
            writeTypeAux(protocol, IEnumTypes::I_VOID);
        },
        [](protocol::IProtocol &protocol, const api::IJsonValue &obj) {//I_BOOL = 0x1
            writeTypeAux(protocol, IEnumTypes::I_BOOL);
            IWriterType<bool>()(protocol, obj.getBoolean());
        },
        nullptr,//I_I08 = 0x2,
        nullptr,//I_I16 = 0x3,
        nullptr,//I_I32 = 0x4,
        [](protocol::IProtocol &protocol, const api::IJsonValue &obj) {//I_I64 = 0x5
            writeTypeAux(protocol, IEnumTypes::I_I64);
            IWriterType<int64_t>()(protocol, obj.getInteger());
        },
        [](protocol::IProtocol &protocol, const api::IJsonValue &obj) {//I_DOUBLE = 0x6
            writeTypeAux(protocol, IEnumTypes::I_DOUBLE);
            IWriterType<double>()(protocol, obj.getDouble());
        },
        [](protocol::IProtocol &protocol, const api::IJsonValue &obj) {//I_STRING = 0x7
            writeTypeAux(protocol, IEnumTypes::I_STRING);
            IWriterType<std::string>()(protocol, obj.getString());
        },
        [](protocol::IProtocol &protocol, const api::IJsonValue &obj) {//I_LIST = 0x8
            writeTypeAux(protocol, IEnumTypes::I_LIST);
            IWriterType<std::vector<api::IJsonValue>>()(protocol, obj.getArray());
        },
        [](protocol::IProtocol &protocol, const api::IJsonValue &obj) {//I_MAP = 0xa
            writeTypeAux(protocol, IEnumTypes::I_MAP);
            IWriterType<std::unordered_map<std::string, api::IJsonValue>>()(protocol, obj.getMap());
        },
        nullptr,//I_PAIR = 0xb,
        nullptr,//I_BINARY = 0xc,
        nullptr,//I_PAIR_LIST = 0xd,
        nullptr,//I_JSON = 0xe,
};

void (*IReaderType<IJsonValue>::readers[])(protocol::IProtocol &protocol, api::IJsonValue &obj){
        [](protocol::IProtocol &protocol, api::IJsonValue &obj) {//I_VOID = 0x0
            obj.setNull();
        },
        [](protocol::IProtocol &protocol, api::IJsonValue &obj) {//I_BOOL = 0x1
            obj.setBoolean(IReaderType<bool>()(protocol));;
        },
        [](protocol::IProtocol &protocol, api::IJsonValue &obj) {//I_I08 = 0x2,
            obj.setInteger(IReaderType<int8_t>()(protocol));
        },
        [](protocol::IProtocol &protocol, api::IJsonValue &obj) {//I_I16 = 0x3,
            obj.setInteger(IReaderType<int16_t>()(protocol));
        },
        [](protocol::IProtocol &protocol, api::IJsonValue &obj) {//I_I32 = 0x4,
            obj.setInteger(IReaderType<int32_t>()(protocol));
        },
        [](protocol::IProtocol &protocol, api::IJsonValue &obj) {//I_I64 = 0x5
            obj.setInteger(IReaderType<int64_t>()(protocol));
        },
        [](protocol::IProtocol &protocol, api::IJsonValue &obj) {//I_DOUBLE = 0x6
            obj.setDouble(IReaderType<double>()(protocol));
        },
        [](protocol::IProtocol &protocol, api::IJsonValue &obj) {//I_STRING = 0x7
            obj.setString(IReaderType<std::string>()(protocol));
        },
        [](protocol::IProtocol &protocol, api::IJsonValue &obj) {//I_LIST = 0x8
            obj.setArray(IReaderType<std::vector<api::IJsonValue>>()(protocol));
        },
        [](protocol::IProtocol &protocol, api::IJsonValue &obj) {//I_MAP = 0xa
            obj.setMap(IReaderType<std::unordered_map<std::string, api::IJsonValue>>()(protocol));
        },
        [](protocol::IProtocol &protocol, api::IJsonValue &obj) {//I_PAIR = 0xb,
            auto size = readSizeAux(protocol);
            std::vector<api::IJsonValue> vector;
            vector.reserve(size);
            auto r_first = readers[readSizeAux(protocol)];
            auto r_second = readers[readSizeAux(protocol)];
            for (decltype(size) i = 0; i < size; i++) {
                std::unordered_map<std::string, api::IJsonValue> map(2);
                r_first(protocol, map["first"]);
                r_second(protocol, map["second"]);
                vector.emplace_back(std::move(map));
            }
            obj.setArray(std::move(vector));
        },
        [](protocol::IProtocol &protocol, api::IJsonValue &obj) {//I_BINARY = 0xc,
            auto size = readSizeAux(protocol);
            std::string bytes;
            int8_t byte;
            bytes.reserve(size);
            for (decltype(size) i = 0; i < size; i++) {
                protocol.readByte(byte);
                bytes += byte;
            }
            obj.setString(std::move(bytes));
        },
        [](protocol::IProtocol &protocol, api::IJsonValue &obj) {//I_PAIR_LIST = 0xd,
            std::unordered_map<std::string, api::IJsonValue> map(2);
            readers[readTypeAux(protocol)](protocol, map["first"]);
            readers[readTypeAux(protocol)](protocol, map["second"]);
            obj.setMap(map);
        },
        [](protocol::IProtocol &protocol, api::IJsonValue &obj) {//I_JSON = 0xe,
            readers[readTypeAux(protocol)](protocol, obj); //should never be executed
        }
};

void (*IPrinterType<IJsonValue>::printers[])(std::ostream &out, const api::IJsonValue &v, int64_t level){
        [](std::ostream &out, const api::IJsonValue &v, int64_t level) {//I_VOID = 0x0
        },
        [](std::ostream &out, const api::IJsonValue &v, int64_t level) {//I_I08 = 0x2,
            IPrinterType<bool>()(out, v.getBoolean(), level);
        },
        nullptr,//I_I08 = 0x2,
        nullptr,//I_I16 = 0x3,
        nullptr,//I_I32 = 0x4,
        [](std::ostream &out, const api::IJsonValue &v, int64_t level) {//I_I64 = 0x5
            IPrinterType<int64_t>()(out, v.getInteger(), level);
        },
        [](std::ostream &out, const api::IJsonValue &v, int64_t level) {//I_DOUBLE = 0x6
            IPrinterType<double>()(out, v.getDouble(), level);
        },
        [](std::ostream &out, const api::IJsonValue &v, int64_t level) {//I_STRING = 0x7
            IPrinterType<std::string>()(out, v.getString(), level);
        },
        [](std::ostream &out, const api::IJsonValue &v, int64_t level) {//I_LIST = 0x8
            IPrinterType<std::vector<api::IJsonValue>>()(out, v.getArray(), level);
        },
        [](std::ostream &out, const api::IJsonValue &v, int64_t level) {//I_MAP = 0xa
            IPrinterType<std::unordered_map<std::string, api::IJsonValue>>()(out, v.getMap(), level);
        },
        nullptr,//I_PAIR = 0xb,
        nullptr,//I_BINARY = 0xc,
        nullptr,//I_PAIR_LIST = 0xd,
        nullptr,//I_JSON = 0xe,
};