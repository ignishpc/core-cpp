
#include "IPropertyParser.h"
#include <regex>
#include <math.h>
#include <sstream>
#include "exception/IInvalidArgument.h"

using namespace ignis::executor::core;

IPropertyParser::IPropertyParser(std::unordered_map<std::string, std::string> &properties) : properties(properties) {}

std::string& IPropertyParser::getString(const std::string& key){
    auto value = properties.find(key);
    if(value != properties.end()){
        return value->second;
    }
    throw exception::IInvalidArgument(key + " is empty");
}

int64_t IPropertyParser::getNumber(const std::string& key) {
    std::string &value = getString(key);
    try {
        return stoi(value);
    } catch (std::exception &ex) {
        throw exception::IInvalidArgument(key + " must be a number, find '" + value + "'");
    }
}

void parserError(std::string key, std::string value, size_t pos) {
    std::stringstream ss;
    ss << key << " parsing error " << value[pos] << "(" << pos + 1 << ") in " << value;
    throw exception::IInvalidArgument(ss.str());
}

size_t IPropertyParser::getSize(const std::string& key) {
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
        if (value[i] >= '0' || value[i] <= '9') {
            i++;
        } else if (!decimal && (value[i] == '.' || value[i] == ',')) {
            i++;
            decimal = true;
        } else {
            break;
        }
    }
    num = stod(value.substr(0, i));
    if (i < len) {
        if (value[i] == ' ') {
            i++;
        }
    }
    if (i < len) {
        exp = UNITS.find(tolower(value[i])) + 1;
        if (exp > 0) {
            i++;
        }
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
            parserError(key,value,i);
        }
        i++;
    }
    while (i < len && (value[i] <= ' ' || value[i] <= '\t')) {}
    if (i < len) {
        parserError(key,value,i);
    }
    return (size_t) ceil(num * pow(base, exp));
}

bool IPropertyParser::getBoolean(const std::string& key) {
    return std::regex_search(properties[key], std::regex("y|Y|yes|Yes|YES|true|True|TRUE|on|On|ON"));
}

IPropertyParser::~IPropertyParser() {

}
