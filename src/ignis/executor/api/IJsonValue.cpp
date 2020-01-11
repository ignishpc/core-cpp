
#include "IJsonValue.h"
#include "ignis/executor/core/io/IEnumTypes.h"

using namespace ignis::executor::api;
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

IJsonValue::IJsonValue(){
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

int64_t IJsonValue::getDouble() const {
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

