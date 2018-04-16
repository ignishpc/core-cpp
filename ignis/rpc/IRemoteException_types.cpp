/**
 * Autogenerated by Thrift Compiler (0.11.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "IRemoteException_types.h"

#include <algorithm>
#include <ostream>

#include <thrift/TToString.h>

namespace ignis { namespace rpc {


IRemoteException::~IRemoteException() throw() {
}


void IRemoteException::__set_name(const std::string& val) {
  this->name = val;
}

void IRemoteException::__set_stack(const std::string& val) {
  this->stack = val;
}
std::ostream& operator<<(std::ostream& out, const IRemoteException& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t IRemoteException::read(::apache::thrift::protocol::TProtocol* iprot) {

  ::apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_name = false;
  bool isset_stack = false;

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
          xfer += iprot->readString(this->name);
          isset_name = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->stack);
          isset_stack = true;
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

  if (!isset_name)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_stack)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t IRemoteException::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  ::apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("IRemoteException");

  xfer += oprot->writeFieldBegin("name", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeString(this->name);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("stack", ::apache::thrift::protocol::T_STRING, 2);
  xfer += oprot->writeString(this->stack);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(IRemoteException &a, IRemoteException &b) {
  using ::std::swap;
  swap(a.name, b.name);
  swap(a.stack, b.stack);
}

IRemoteException::IRemoteException(const IRemoteException& other0) : TException() {
  name = other0.name;
  stack = other0.stack;
}
IRemoteException& IRemoteException::operator=(const IRemoteException& other1) {
  name = other1.name;
  stack = other1.stack;
  return *this;
}
void IRemoteException::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "IRemoteException(";
  out << "name=" << to_string(name);
  out << ", " << "stack=" << to_string(stack);
  out << ")";
}

const char* IRemoteException::what() const throw() {
  try {
    std::stringstream ss;
    ss << "TException - service has thrown: " << *this;
    this->thriftTExceptionMessageHolder_ = ss.str();
    return this->thriftTExceptionMessageHolder_.c_str();
  } catch (const std::exception&) {
    return "TException - service has thrown: IRemoteException";
  }
}

}} // namespace
