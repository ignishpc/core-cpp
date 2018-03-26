#include "IgnisException.h"


using namespace std;
using namespace ignis::driver::api;

IgnisException::IgnisException(const std::string &message, const std::string &stacktrace)
        : runtime_error(message), stacktrace(stacktrace) {}


std::string &IgnisException::getStacktrace() {
    return stacktrace;
}

void IgnisException::print(std::ostream &stream) {
    stream << string("IgnisException: ") << what() << endl << stacktrace << endl;
}