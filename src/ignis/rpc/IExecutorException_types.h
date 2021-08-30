/**
 * Autogenerated by Thrift Compiler (0.14.1)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef IExecutorException_TYPES_H
#define IExecutorException_TYPES_H

#include <iosfwd>

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/TBase.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include <functional>
#include <memory>


namespace ignis { namespace rpc {

class IExecutorException;


class IExecutorException : public ::apache::thrift::TException {
 public:

  IExecutorException(const IExecutorException&);
  IExecutorException& operator=(const IExecutorException&);
  IExecutorException() : message(), cause_() {
  }

  virtual ~IExecutorException() noexcept;
  std::string message;
  std::string cause_;

  void __set_message(const std::string& val);

  void __set_cause_(const std::string& val);

  bool operator == (const IExecutorException & rhs) const
  {
    if (!(message == rhs.message))
      return false;
    if (!(cause_ == rhs.cause_))
      return false;
    return true;
  }
  bool operator != (const IExecutorException &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const IExecutorException & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
  mutable std::string thriftTExceptionMessageHolder_;
  const char* what() const noexcept;
};

void swap(IExecutorException &a, IExecutorException &b);

std::ostream& operator<<(std::ostream& out, const IExecutorException& obj);

}} // namespace

#endif
