
#include "IPropertyParser.h"
#include "exception/IInvalidArgument.h"
#include <math.h>
#include <regex>
#include <sstream>

using namespace ignis::executor::core;

IPropertyParser::IPropertyParser(std::unordered_map<std::string, std::string> &properties) : properties(properties) {}

std::string &IPropertyParser::getString(const std::string &key) {
    auto value = properties.find(key);
    if (value != properties.end()) { return value->second; }
    throw exception::IInvalidArgument(key + " is empty");
}

int64_t IPropertyParser::getNumber(const std::string &key) {
    std::string &value = getString(key);
    try {
        size_t end;
        int64_t n = stol(value, &end);
        if (value.length() != end) { parserError(key, value, end); }
        return n;
    } catch (std::exception &ex) { throw exception::IInvalidArgument(key + " must be a number, find '" + value + "'"); }
}

int64_t IPropertyParser::getMinNumber(const std::string &key, const int64_t &min) {
    auto value = getNumber(key);
    if (value < min) {
        std::stringstream ss;
        ss << key << " error " << value << " is less than " << value;
        throw exception::IInvalidArgument(ss.str());
    }
    return value;
}

int64_t IPropertyParser::getMaxNumber(const std::string &key, const int64_t &max) {
    auto value = getNumber(key);
    if (value > max) {
        std::stringstream ss;
        ss << key << " error " << value << " is greater than " << value;
        throw exception::IInvalidArgument(ss.str());
    }
    return value;
}

int64_t IPropertyParser::getRangeNumber(const std::string &key, const int64_t &min, const int64_t &max) {
    auto value = getMinNumber(key, min);
    if (value > max) {
        std::stringstream ss;
        ss << key << " error " << value << " is greater than " << value;
        throw exception::IInvalidArgument(ss.str());
    }
    return value;
}

void IPropertyParser::parserError(const std::string &key, const std::string &value, size_t pos) {
    std::stringstream ss;
    ss << key << " parsing error " << value[pos] << "(" << pos + 1 << ") in " << value;
    throw exception::IInvalidArgument(ss.str());
}

size_t IPropertyParser::getSize(const std::string &key) {
    std::string &value = getString(key);
    std::string UNITS = "KMGTPEZY";
    double num;
    size_t base;
    size_t exp;
    bool decimal = false;
    size_t i = 0;
    size_t len = value.length();
    while (i < len && value[i] <= ' ') {}
    while (i < len) {
        if (value[i] >= '0' && value[i] <= '9') {
            i++;
        } else if (!decimal && (value[i] == '.' || value[i] == ',')) {
            i++;
            decimal = true;
        } else {
            break;
        }
    }
    size_t end;
    num = stod(value.substr(0, i), &end);
    if (i != end) { parserError(key, value, end); }
    if (i < len) {
        if (value[i] == ' ') { i++; }
    }
    if (i < len) {
        exp = UNITS.find(toupper(value[i])) + 1;
        if (exp > 0) { i++; }
    }
    if (i < len && exp > 0 && value[i] == 'i') {
        i++;
        base = 1024;
    } else {
        base = 1000;
    }
    if (i < len) {
        if (value[i] == 'B') {
            //Nothing
        } else if (value[i] == 'b') {
            num = num / 8;
        } else {
            parserError(key, value, i);
        }
        i++;
    }
    while (i < len && (value[i] <= ' ' || value[i] <= '\t')) {}
    if (i < len) { parserError(key, value, i); }
    return (size_t) ceil(num * pow(base, exp));
}

bool IPropertyParser::getBoolean(const std::string &key) {
    return std::regex_search(properties[key], std::regex("y|Y|yes|Yes|YES|true|True|TRUE|on|On|ON"));
}

IPropertyParser::~IPropertyParser() {}
