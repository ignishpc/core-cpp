#include "IgnisExecutorException.h"
#include <sstream>

using namespace std;
using namespace ignis::executor::api;

IgnisExecutorException::IgnisExecutorException(const string &message)
        : runtime_error(message),
          stacktrace(boost::stacktrace::stacktrace()) {
    setExceptionName();
}

IgnisExecutorException::IgnisExecutorException(const char *message)
        : runtime_error(message),
          stacktrace(boost::stacktrace::stacktrace()) {
    setExceptionName();
}

IgnisExecutorException::IgnisExecutorException(const string &message, IgnisExecutorException &cause)
        : runtime_error(message),
          cause(make_shared<IgnisExecutorException>(cause)),
          stacktrace(boost::stacktrace::stacktrace()) {
    setExceptionName();
}

IgnisExecutorException::IgnisExecutorException(const char *message, IgnisExecutorException &cause)
        : runtime_error(message),
          cause(make_shared<IgnisExecutorException>(cause)),
          stacktrace(boost::stacktrace::stacktrace()) {
    setExceptionName();
}


bool IgnisExecutorException::hasCause() {
    return bool(cause);
}

IgnisExecutorException &IgnisExecutorException::getCause() {
    return *cause;
}

void IgnisExecutorException::printStackTrace(std::ostream &stream) {
    stream << stacktrace;
}

std::string IgnisExecutorException::getStacktrace() {
    stringstream ss;
    printStackTrace(ss);
    return ss.str();
}

void IgnisExecutorException::print(std::ostream &stream) {
    stream << class_name << ": " << what() << endl;
    printStackTrace(stream);
    if (hasCause()) {
        stream << "Caused by: ";
        stream << cause->toString();
    }
}

void IgnisExecutorException::setCause(std::shared_ptr<IgnisExecutorException> &cause) {
    this->cause = cause;
}

std::string IgnisExecutorException::toString() {
    stringstream ss;
    print(ss);
    return ss.str();
}

IgnisExecutorException::~IgnisExecutorException() {
}







