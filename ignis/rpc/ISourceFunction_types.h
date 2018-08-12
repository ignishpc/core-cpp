/**
 * Autogenerated by Thrift Compiler (0.11.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef ISourceFunction_TYPES_H
#define ISourceFunction_TYPES_H

#include <iosfwd>

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/TBase.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include <thrift/stdcxx.h>


namespace ignis { namespace rpc {

class ISourceFunction;

typedef struct _ISourceFunction__isset {
  _ISourceFunction__isset() : name(false), bytes(false) {}
  bool name :1;
  bool bytes :1;
} _ISourceFunction__isset;

class ISourceFunction : public virtual ::apache::thrift::TBase {
 public:

  ISourceFunction(const ISourceFunction&);
  ISourceFunction& operator=(const ISourceFunction&);
  ISourceFunction() : name(), bytes() {
  }

  virtual ~ISourceFunction() throw();
  std::string name;
  std::string bytes;

  _ISourceFunction__isset __isset;

  void __set_name(const std::string& val);

  void __set_bytes(const std::string& val);

  bool operator == (const ISourceFunction & rhs) const
  {
    if (__isset.name != rhs.__isset.name)
      return false;
    else if (__isset.name && !(name == rhs.name))
      return false;
    if (__isset.bytes != rhs.__isset.bytes)
      return false;
    else if (__isset.bytes && !(bytes == rhs.bytes))
      return false;
    return true;
  }
  bool operator != (const ISourceFunction &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ISourceFunction & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(ISourceFunction &a, ISourceFunction &b);

std::ostream& operator<<(std::ostream& out, const ISourceFunction& obj);

}} // namespace

#endif