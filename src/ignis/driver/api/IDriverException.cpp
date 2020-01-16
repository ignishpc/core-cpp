
#include "IDriverException.h"

using namespace ignis::driver::api;

IDriverException::IDriverException(const std::string &message) : IDriverException(message, "") {}

IDriverException::IDriverException(const std::string &message, const std::string &stacktrace)
        : runtime_error(message), cause(cause) {}


bool IDriverException::hasCause(){
    return !cause.empty();
}

const std::string &IDriverException::getCause() {
    return cause;
}

std::ostream & IDriverException::operator<<(std::ostream &stream) {
    stream << "IDriverException: " << what() << std::endl;
    if(!cause.empty()){
        stream << "Caused by: " + cause;
    }
}

IDriverException::~IDriverException() {}
