
#include "IDynamicTypes.h"

#include <boost/filesystem.hpp>
#include <boost/process/search_path.hpp>
#include <boost/interprocess/sync/file_lock.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <memory>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include "ignis/executor/core/transport/IZlibTransport.h"
#include "ignis/executor/core/protocol/IObjectProtocol.h"
#include "ignis/executor/core/RTTInfo.h"
#include "ignis/executor/api/IJsonValue.h"
#include <iostream>
#include <fstream>

using namespace ignis::executor::core;


std::string IDynamicTypes::typeFromBytes(std::shared_ptr<transport::ITransport> &trans, bool inColl) {
    auto zlib = std::make_shared<transport::IZlibTransport>(trans);
    protocol::IObjectProtocol protocol(zlib);

    std::string typeName;
    protocol.readSerialization();
    int8_t tp;
    protocol.readByte(tp);
    if (inColl) {
        if (tp == io::IEnumTypes::I_LIST || tp == io::IEnumTypes::I_SET) {
            int64_t sz;
            protocol.readI64(sz);
            int8_t tp;
            protocol.readByte(tp);
            auto typeReader = knownTypes[checkRange(tp)];
            std::string nameTp;
            while (sz > 0) {
                sz--;
                typeReader(protocol, nameTp);
                if (!nameTp.empty()) {
                    return nameTp;
                }
            }
        } else if (tp == io::IEnumTypes::I_PAIR_LIST || tp == io::IEnumTypes::I_MAP) {
            int64_t sz;
            protocol.readI64(sz);
            int8_t tpFirst, tpSecond;
            protocol.readByte(tpFirst);
            protocol.readByte(tpSecond);
            auto firstReader = knownTypes[checkRange(tpFirst)];
            auto secondReader = knownTypes[checkRange(tpSecond)];
            std::string nameFirst;
            std::string nameSecond;

            while (sz > 0) {
                sz--;
                firstReader(protocol, nameFirst);
                secondReader(protocol, nameSecond);
                if (!nameFirst.empty() && !nameSecond.empty()) {
                    typeName = RTTInfo::from<std::pair<bool, bool>>().getStandardName();
                    boost::replace_first(typeName, "bool", nameFirst);
                    boost::replace_last(typeName, "bool", nameSecond);
                }
            }
        } else if (tp == io::IEnumTypes::I_BINARY) {
            RTTInfo::from<uint8_t>().getStandardName();
        } else {
            throw exception::ILogicError("C++ type is not a collection");
        }
    } else {
        knownTypes[checkRange(tp)](protocol, typeName);
    }
    if (typeName.empty()) {
        throw exception::ILogicError("C++ cannot auto detect this storage type");
    }
    return typeName;
}

std::string IDynamicTypes::compiler(const std::string &type, const std::string &folder) {
    if (std::system("mpic++ --version") == 0) {
        throw exception::ILogicError("mpic++ compiler not found in path");
    }
    auto fileName = type;
    boost::replace_all(fileName, "::", ".");
    boost::replace_all(fileName, "<", "(");
    boost::replace_all(fileName, ">", ")");
    boost::interprocess::file_lock lock(folder.c_str());
    boost::interprocess::scoped_lock<boost::interprocess::file_lock> slock(lock);
    auto fileSource = folder + "/" + fileName + ".cpp";
    auto fileLibrary = folder + "/" + fileName + ".so";
    auto libraryName = "AutoType:" + fileLibrary;

    if (boost::filesystem::exists(fileLibrary)) {
        return libraryName;
    }

    auto sourceCode = std::string() +
                      "#include <ignis/executor/core/io/IReader.h> //include all default headers\n"
                      "#include <ignis/executor/api/IVector.h>\n" +
                      "#include <ignis/executor/api/IJson.h>\n" +
                      "#include <ignis/executor/api/function/IVoidFunction.h>\n" +
                      "\n" +
                      "class AutoType : public ignis::executor::api::function::IVoidFunction<" + type + "> {};" +
                      "\n" +
                      "ignis_export(AutoType, AutoType)" +
                      "\n";

    std::ofstream file(fileSource, std::ifstream::out | std::ifstream::binary | std::fstream::trunc);
    file << sourceCode;
    file.close();


    int error = std::system(("mpic++ -O3 -shared -fPIC -o -ligniscore" + fileLibrary + " " + fileSource).c_str());
    if (error != 0) {
        throw exception::ILogicError("mpic++ exited with exit value " + std::to_string(error) + " " +
                                     fileSource);
    }

    return libraryName;
}

int8_t IDynamicTypes::checkRange(int8_t tp) {
    if (tp < 0 || tp > 0xe) {
        throw exception::ILogicError("C++ cannot auto detect custom types");
    }
    return tp;
}


void (*IDynamicTypes::knownTypes[])(protocol::IProtocol &protocol, std::string &name) = {
        [](protocol::IProtocol &protocol, std::string &name) {//I_VOID = 0x0
            name = "";
        },
        [](protocol::IProtocol &protocol, std::string &name) {//I_BOOL = 0x1
            bool b;
            protocol.readBool(b);
            name = "bool";
        },
        [](protocol::IProtocol &protocol, std::string &name) {//I_I08 = 0x2,
            int8_t i;
            protocol.readByte(i);
            name = RTTInfo::from<int8_t>().getStandardName();
        },
        [](protocol::IProtocol &protocol, std::string &name) {//I_I16 = 0x3,
            int16_t i;
            protocol.readI16(i);
            name = RTTInfo::from<int16_t>().getStandardName();
        },
        [](protocol::IProtocol &protocol, std::string &name) {//I_I32 = 0x4,
            int32_t i;
            protocol.readI32(i);
            name = RTTInfo::from<int32_t>().getStandardName();
        },
        [](protocol::IProtocol &protocol, std::string &name) {//I_I64 = 0x5
            int64_t i;
            protocol.readI64(i);
            name = RTTInfo::from<int64_t>().getStandardName();
        },
        [](protocol::IProtocol &protocol, std::string &name) {//I_DOUBLE = 0x6
            double d;
            protocol.readDouble(d);
            name = "double";
        },
        [](protocol::IProtocol &protocol, std::string &name) {//I_STRING = 0x7
            std::string s;
            protocol.readString(s);
            name = RTTInfo::from<std::string>().getStandardName();
        },
        [](protocol::IProtocol &protocol, std::string &name) {//I_LIST = 0x8
            int64_t sz;
            protocol.readI64(sz);
            int8_t tp;
            protocol.readByte(tp);
            auto typeReader = knownTypes[checkRange(tp)];
            std::string nameTp;
            while (sz > 0) {
                sz--;
                typeReader(protocol, nameTp);
            }
            if (nameTp.empty() || !name.empty()) {
                return;
            }
            name = RTTInfo::from<std::vector<bool>>().getStandardName();
            boost::replace_first(name, "bool", name);
        },
        [](protocol::IProtocol &protocol, std::string &name) {//I_SET = 0x9,
            int64_t sz;
            protocol.readI64(sz);
            int8_t tp;
            protocol.readByte(tp);
            auto typeReader = knownTypes[checkRange(tp)];
            std::string nameTp;
            while (sz > 0) {
                sz--;
                typeReader(protocol, nameTp);
            }
            if (nameTp.empty() || !name.empty()) {
                return;
            }
            name = RTTInfo::from<std::unordered_set<bool>>().getStandardName();
            boost::replace_first(name, "bool", name);
        },
        [](protocol::IProtocol &protocol, std::string &name) {//I_MAP = 0xa
            int64_t sz;
            protocol.readI64(sz);
            int8_t tpKey, tpValue;
            protocol.readByte(tpKey);
            protocol.readByte(tpValue);
            auto keyReader = knownTypes[checkRange(tpKey)];
            auto valueReader = knownTypes[checkRange(tpValue)];
            std::string nameKey;
            std::string nameValue;

            while (sz > 0) {
                sz--;
                keyReader(protocol, nameKey);
                valueReader(protocol, nameValue);
            }
            if (nameKey.empty() || nameValue.empty() || !name.empty()) {
                return;
            }
            name = RTTInfo::from<std::unordered_map<bool, bool>>().getStandardName();
            boost::replace_first(name, "bool", nameKey);
            boost::replace_last(name, "bool", nameValue);
        },
        [](protocol::IProtocol &protocol, std::string &name) {//I_PAIR = 0xb,
            int8_t firstKey, secondValue;
            protocol.readByte(firstKey);
            protocol.readByte(secondValue);
            std::string nameFirst;
            knownTypes[checkRange(firstKey)](protocol, nameFirst);
            std::string nameSecond;
            knownTypes[checkRange(secondValue)](protocol, nameSecond);
            if (nameFirst.empty() || nameSecond.empty() || !name.empty()) {
                return;
            }
            auto pairName = RTTInfo::from<std::pair<bool, double>>().getStandardName();
            boost::replace_first(pairName, "bool", nameFirst);
            boost::replace_last(pairName, "bool", nameSecond);
            name = pairName;
        },
        [](protocol::IProtocol &protocol, std::string &name) {//I_BINARY = 0xc,
            name = RTTInfo::from<std::vector<uint8_t>>().getStandardName();
        },
        [](protocol::IProtocol &protocol, std::string &name) {//I_PAIR_LIST = 0xd,
            int64_t sz;
            protocol.readI64(sz);
            int8_t tpFirst, tpSecond;
            protocol.readByte(tpFirst);
            protocol.readByte(tpSecond);
            auto firstReader = knownTypes[checkRange(tpFirst)];
            auto secondReader = knownTypes[checkRange(tpSecond)];
            std::string nameFirst;
            std::string nameSecond;

            while (sz > 0) {
                sz--;
                firstReader(protocol, nameFirst);
                secondReader(protocol, nameSecond);
            }
            if (nameFirst.empty() || nameSecond.empty() || !name.empty()) {
                return;
            }
            name = RTTInfo::from<std::vector<std::pair<bool, bool>>>().getStandardName();
            boost::replace_first(name,"bool", nameFirst);
            boost::replace_last(name, "bool", nameSecond);
        },
        [](protocol::IProtocol &protocol, std::string &name) {//I_JSON = 0xe,
            name = RTTInfo::from<executor::api::IJsonValue>().getStandardName();
        }
};

