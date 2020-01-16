/**
 * Autogenerated by Thrift Compiler (0.13.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "IDriverException_types.h"

#include <algorithm>
#include <ostream>

#include <thrift/TToString.h>

namespace ignis { namespace rpc { namespace driver {


IDriverException::~IDriverException() noexcept {
}


void IDriverException::__set_message(const std::string& val) {
  this->message = val;
}

void IDriverException::__set__cause(const std::string& val) {
  this->_cause = val;
}
std::ostream& operator<<(std::ostream& out, const IDriverException& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t IDriverException::read(::apache::thrift::protocol::TProtocol* iprot) {

  ::apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_message = false;
  bool isset__cause = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->message);
          isset_message = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->_cause);
          isset__cause = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_message)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset__cause)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t IDriverException::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  ::apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("IDriverException");

  xfer += oprot->writeFieldBegin("message", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeString(this->message);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("_cause", ::apache::thrift::protocol::T_STRING, 2);
  xfer += oprot->writeString(this->_cause);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(IDriverException &a, IDriverException &b) {
  using ::std::swap;
  swap(a.message, b.message);
  swap(a._cause, b._cause);
}

IDriverException::IDriverException(const IDriverException& other0) : TException() {
  message = other0.message;
  _cause = other0._cause;
}
IDriverException& IDriverException::operator=(const IDriverException& other1) {
  message = other1.message;
  _cause = other1._cause;
  return *this;
}
void IDriverException::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "IDriverException(";
  out << "message=" << to_string(message);
  out << ", " << "_cause=" << to_string(_cause);
  out << ")";
}

const char* IDriverException::what() const noexcept {
  try {
    std::stringstream ss;
    ss << "TException - service has thrown: " << *this;
    this->thriftTExceptionMessageHolder_ = ss.str();
    return this->thriftTExceptionMessageHolder_.c_str();
  } catch (const std::exception&) {
    return "TException - service has thrown: IDriverException";
  }
}

}}} // namespace
