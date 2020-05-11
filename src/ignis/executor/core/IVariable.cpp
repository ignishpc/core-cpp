
#include "IVariable.h"
#include "ignis/executor/core/ILibraryLoader.h"
#include "ignis/executor/core/transport/IMemoryBuffer.h"
#include "ignis/executor/core/transport/IZlibTransport.h"

using namespace ignis::executor::core;

IBytesVariable &IVariable::bytes() { return reinterpret_cast<IBytesVariable &>(*this); }

IBytesVariable::IBytesVariable(const std::string &value) : value(value) {}

IBytesVariable::IBytesVariable(std::string &&value) : value(value) {}

IBytesVariable::operator bool() { return false; }

std::shared_ptr<protocol::IObjectProtocol> IBytesVariable::protocol() {
    char *cbytes = const_cast<char *>(value.c_str());
    auto buffer = std::make_shared<core::transport::IMemoryBuffer>((uint8_t *) cbytes, value.size());
    auto zlib = std::make_shared<core::transport::IZlibTransport>(buffer);
    return std::make_shared<core::protocol::IObjectProtocol>(zlib);
}