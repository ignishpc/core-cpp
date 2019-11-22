#include "IException.h"
#include <sstream>

#define  BOOST_STACKTRACE_USE_BACKTRACE

#include <boost/stacktrace.hpp>

using namespace ignis::executor::core::exception;

IException::IException(const IException &other) :
        runtime_error(other.what()), stacktrace(other.stacktrace), cause(other.cause) {}

IException::IException(const std::string &message)
        : runtime_error(message),
          stacktrace(std::make_shared<boost::stacktrace::stacktrace>(boost::stacktrace::stacktrace())) {}

IException::IException(const std::string &message, const IException &cause)
        : runtime_error(message),
          stacktrace(std::make_shared<boost::stacktrace::stacktrace>(boost::stacktrace::stacktrace())),
          cause(std::make_shared<IException>(cause)) {}

IException::IException(const std::exception &cause) : IException(cause.what()) {}

bool IException::hasCause() const {
    return bool(cause);
}

IException const &IException::getCause() const {
    return hasCause() ? *cause : *this;
}

void IException::printStackTrace(std::ostream &stream) const {
    stream << exceptionName() << ": " << what() << std::endl;
    stream << *std::static_pointer_cast<boost::stacktrace::stacktrace>(stacktrace);
    if (hasCause()) {
        stream << "Caused by: ";
        stream << cause;
    }
}

std::string IException::getStacktrace() const {
    std::stringstream ss;
    printStackTrace(ss);
    return ss.str();
}

void IException::print(std::ostream &stream) const {
    printStackTrace(stream);
}

std::string IException::toString() const {
    std::stringstream ss;
    print(ss);
    return ss.str();
}

std::ostream &IException::operator<<(std::ostream &stream) const {
    print(stream);
}

std::string IException::exceptionName() const {
    return parseName<IException>();
}

IException::~IException() {
}







