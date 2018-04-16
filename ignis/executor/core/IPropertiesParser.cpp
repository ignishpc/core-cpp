
#include "IPropertiesParser.h"
#include <regex>
#include <math.h>
#include <sstream>
#include "../../exceptions/IInvalidArgument.h"

using namespace std;
using namespace ignis::executor::core;

IPropertiesParser::IPropertiesParser(unordered_map<string, string> &properties) : properties(properties) {}

size_t IPropertiesParser::getNumber(std::string key) {
    string &value = properties[key];
    try {
        return stoi(value);
    } catch (exception &ex) {
        throw exceptions::IInvalidArgument(key + " must be a number, find '" + value + "'");
    }
}

void parserError(std::string key, std::string value, size_t pos) {
    stringstream ss;
    ss << key << " parsing error " << value[pos] << "(" << pos + 1 << ") in " << value;
    throw ignis::exceptions::IInvalidArgument(ss.str());
}

size_t IPropertiesParser::getSize(std::string key) {
    string &value = properties[key];
    string UNITS = "KMGTPEZY";
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

bool IPropertiesParser::getBoolean(std::string key) {
    return std::regex_search(properties[key], regex("y|Y|yes|Yes|YES|true|True|TRUE|on|On|ON"));
}

IPropertiesParser::~IPropertiesParser() {

}
