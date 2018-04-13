#include "IException.h"
#include <sstream>

using namespace std;
using namespace ignis::exceptions;

IException::IException(const string &message)
        : runtime_error(message),
          stacktrace(boost::stacktrace::stacktrace()) {
    setExceptionName(this);
}

IException::IException(const char *message)
        : runtime_error(message),
          stacktrace(boost::stacktrace::stacktrace()) {
    setExceptionName(this);
}

IException::IException(const string &message, const IException &cause)
        : runtime_error(message),
          cause(make_shared<IException>(cause)),
          stacktrace(boost::stacktrace::stacktrace()) {
    setExceptionName(this);
}

IException::IException(const char *message, const IException &cause)
        : runtime_error(message),
          cause(make_shared<IException>(cause)),
          stacktrace(boost::stacktrace::stacktrace()) {
    setExceptionName(this);
}


bool IException::hasCause() const {
    return bool(cause);
}

IException &IException::getCause() {
    return *cause;
}

void IException::printStackTrace(std::ostream &stream) const {
    stream << stacktrace;
}

std::string IException::getStacktrace() const {
    stringstream ss;
    printStackTrace(ss);
    return ss.str();
}

void IException::print(std::ostream &stream) const {
    stream << class_name << ": " << what() << endl;
    printStackTrace(stream);
    if (hasCause()) {
        stream << "Caused by: ";
        stream << cause->toString();
    }
}

void IException::setCause(std::shared_ptr<IException> &cause) {
    this->cause = cause;
}

std::string IException::toString() const {
    stringstream ss;
    print(ss);
    return ss.str();
}

std::ostream& IException::operator<<(std::ostream &stream) const{
    print(stream);
}

IException::~IException() {
}







