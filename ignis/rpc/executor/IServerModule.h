/**
 * Autogenerated by Thrift Compiler (0.11.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef IServerModule_H
#define IServerModule_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include "IServerModule_types.h"

namespace ignis { namespace rpc { namespace executor {

#ifdef _MSC_VER
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class IServerModuleIf {
 public:
  virtual ~IServerModuleIf() {}
  virtual void updateProperties(const std::map<std::string, std::string> & properties) = 0;
  virtual void stop() = 0;
  virtual bool test() = 0;
};

class IServerModuleIfFactory {
 public:
  typedef IServerModuleIf Handler;

  virtual ~IServerModuleIfFactory() {}

  virtual IServerModuleIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(IServerModuleIf* /* handler */) = 0;
};

class IServerModuleIfSingletonFactory : virtual public IServerModuleIfFactory {
 public:
  IServerModuleIfSingletonFactory(const ::apache::thrift::stdcxx::shared_ptr<IServerModuleIf>& iface) : iface_(iface) {}
  virtual ~IServerModuleIfSingletonFactory() {}

  virtual IServerModuleIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(IServerModuleIf* /* handler */) {}

 protected:
  ::apache::thrift::stdcxx::shared_ptr<IServerModuleIf> iface_;
};

class IServerModuleNull : virtual public IServerModuleIf {
 public:
  virtual ~IServerModuleNull() {}
  void updateProperties(const std::map<std::string, std::string> & /* properties */) {
    return;
  }
  void stop() {
    return;
  }
  bool test() {
    bool _return = false;
    return _return;
  }
};

typedef struct _IServerModule_updateProperties_args__isset {
  _IServerModule_updateProperties_args__isset() : properties(false) {}
  bool properties :1;
} _IServerModule_updateProperties_args__isset;

class IServerModule_updateProperties_args {
 public:

  IServerModule_updateProperties_args(const IServerModule_updateProperties_args&);
  IServerModule_updateProperties_args& operator=(const IServerModule_updateProperties_args&);
  IServerModule_updateProperties_args() {
  }

  virtual ~IServerModule_updateProperties_args() throw();
  std::map<std::string, std::string>  properties;

  _IServerModule_updateProperties_args__isset __isset;

  void __set_properties(const std::map<std::string, std::string> & val);

  bool operator == (const IServerModule_updateProperties_args & rhs) const
  {
    if (!(properties == rhs.properties))
      return false;
    return true;
  }
  bool operator != (const IServerModule_updateProperties_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const IServerModule_updateProperties_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class IServerModule_updateProperties_pargs {
 public:


  virtual ~IServerModule_updateProperties_pargs() throw();
  const std::map<std::string, std::string> * properties;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _IServerModule_updateProperties_result__isset {
  _IServerModule_updateProperties_result__isset() : ex(false) {}
  bool ex :1;
} _IServerModule_updateProperties_result__isset;

class IServerModule_updateProperties_result {
 public:

  IServerModule_updateProperties_result(const IServerModule_updateProperties_result&);
  IServerModule_updateProperties_result& operator=(const IServerModule_updateProperties_result&);
  IServerModule_updateProperties_result() {
  }

  virtual ~IServerModule_updateProperties_result() throw();
   ::ignis::rpc::IRemoteException ex;

  _IServerModule_updateProperties_result__isset __isset;

  void __set_ex(const  ::ignis::rpc::IRemoteException& val);

  bool operator == (const IServerModule_updateProperties_result & rhs) const
  {
    if (!(ex == rhs.ex))
      return false;
    return true;
  }
  bool operator != (const IServerModule_updateProperties_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const IServerModule_updateProperties_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _IServerModule_updateProperties_presult__isset {
  _IServerModule_updateProperties_presult__isset() : ex(false) {}
  bool ex :1;
} _IServerModule_updateProperties_presult__isset;

class IServerModule_updateProperties_presult {
 public:


  virtual ~IServerModule_updateProperties_presult() throw();
   ::ignis::rpc::IRemoteException ex;

  _IServerModule_updateProperties_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};


class IServerModule_stop_args {
 public:

  IServerModule_stop_args(const IServerModule_stop_args&);
  IServerModule_stop_args& operator=(const IServerModule_stop_args&);
  IServerModule_stop_args() {
  }

  virtual ~IServerModule_stop_args() throw();

  bool operator == (const IServerModule_stop_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const IServerModule_stop_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const IServerModule_stop_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class IServerModule_stop_pargs {
 public:


  virtual ~IServerModule_stop_pargs() throw();

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _IServerModule_stop_result__isset {
  _IServerModule_stop_result__isset() : ex(false) {}
  bool ex :1;
} _IServerModule_stop_result__isset;

class IServerModule_stop_result {
 public:

  IServerModule_stop_result(const IServerModule_stop_result&);
  IServerModule_stop_result& operator=(const IServerModule_stop_result&);
  IServerModule_stop_result() {
  }

  virtual ~IServerModule_stop_result() throw();
   ::ignis::rpc::IRemoteException ex;

  _IServerModule_stop_result__isset __isset;

  void __set_ex(const  ::ignis::rpc::IRemoteException& val);

  bool operator == (const IServerModule_stop_result & rhs) const
  {
    if (!(ex == rhs.ex))
      return false;
    return true;
  }
  bool operator != (const IServerModule_stop_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const IServerModule_stop_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _IServerModule_stop_presult__isset {
  _IServerModule_stop_presult__isset() : ex(false) {}
  bool ex :1;
} _IServerModule_stop_presult__isset;

class IServerModule_stop_presult {
 public:


  virtual ~IServerModule_stop_presult() throw();
   ::ignis::rpc::IRemoteException ex;

  _IServerModule_stop_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};


class IServerModule_test_args {
 public:

  IServerModule_test_args(const IServerModule_test_args&);
  IServerModule_test_args& operator=(const IServerModule_test_args&);
  IServerModule_test_args() {
  }

  virtual ~IServerModule_test_args() throw();

  bool operator == (const IServerModule_test_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const IServerModule_test_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const IServerModule_test_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class IServerModule_test_pargs {
 public:


  virtual ~IServerModule_test_pargs() throw();

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _IServerModule_test_result__isset {
  _IServerModule_test_result__isset() : success(false), ex(false) {}
  bool success :1;
  bool ex :1;
} _IServerModule_test_result__isset;

class IServerModule_test_result {
 public:

  IServerModule_test_result(const IServerModule_test_result&);
  IServerModule_test_result& operator=(const IServerModule_test_result&);
  IServerModule_test_result() : success(0) {
  }

  virtual ~IServerModule_test_result() throw();
  bool success;
   ::ignis::rpc::IRemoteException ex;

  _IServerModule_test_result__isset __isset;

  void __set_success(const bool val);

  void __set_ex(const  ::ignis::rpc::IRemoteException& val);

  bool operator == (const IServerModule_test_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    if (!(ex == rhs.ex))
      return false;
    return true;
  }
  bool operator != (const IServerModule_test_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const IServerModule_test_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _IServerModule_test_presult__isset {
  _IServerModule_test_presult__isset() : success(false), ex(false) {}
  bool success :1;
  bool ex :1;
} _IServerModule_test_presult__isset;

class IServerModule_test_presult {
 public:


  virtual ~IServerModule_test_presult() throw();
  bool* success;
   ::ignis::rpc::IRemoteException ex;

  _IServerModule_test_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class IServerModuleClient : virtual public IServerModuleIf {
 public:
  IServerModuleClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  IServerModuleClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void updateProperties(const std::map<std::string, std::string> & properties);
  void send_updateProperties(const std::map<std::string, std::string> & properties);
  void recv_updateProperties();
  void stop();
  void send_stop();
  void recv_stop();
  bool test();
  void send_test();
  bool recv_test();
 protected:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class IServerModuleProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  ::apache::thrift::stdcxx::shared_ptr<IServerModuleIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (IServerModuleProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_updateProperties(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_stop(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_test(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  IServerModuleProcessor(::apache::thrift::stdcxx::shared_ptr<IServerModuleIf> iface) :
    iface_(iface) {
    processMap_["updateProperties"] = &IServerModuleProcessor::process_updateProperties;
    processMap_["stop"] = &IServerModuleProcessor::process_stop;
    processMap_["test"] = &IServerModuleProcessor::process_test;
  }

  virtual ~IServerModuleProcessor() {}
};

class IServerModuleProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  IServerModuleProcessorFactory(const ::apache::thrift::stdcxx::shared_ptr< IServerModuleIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::apache::thrift::stdcxx::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::apache::thrift::stdcxx::shared_ptr< IServerModuleIfFactory > handlerFactory_;
};

class IServerModuleMultiface : virtual public IServerModuleIf {
 public:
  IServerModuleMultiface(std::vector<apache::thrift::stdcxx::shared_ptr<IServerModuleIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~IServerModuleMultiface() {}
 protected:
  std::vector<apache::thrift::stdcxx::shared_ptr<IServerModuleIf> > ifaces_;
  IServerModuleMultiface() {}
  void add(::apache::thrift::stdcxx::shared_ptr<IServerModuleIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void updateProperties(const std::map<std::string, std::string> & properties) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->updateProperties(properties);
    }
    ifaces_[i]->updateProperties(properties);
  }

  void stop() {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->stop();
    }
    ifaces_[i]->stop();
  }

  bool test() {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->test();
    }
    return ifaces_[i]->test();
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class IServerModuleConcurrentClient : virtual public IServerModuleIf {
 public:
  IServerModuleConcurrentClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  IServerModuleConcurrentClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void updateProperties(const std::map<std::string, std::string> & properties);
  int32_t send_updateProperties(const std::map<std::string, std::string> & properties);
  void recv_updateProperties(const int32_t seqid);
  void stop();
  int32_t send_stop();
  void recv_stop(const int32_t seqid);
  bool test();
  int32_t send_test();
  bool recv_test(const int32_t seqid);
 protected:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
  ::apache::thrift::async::TConcurrentClientSyncInfo sync_;
};

#ifdef _MSC_VER
  #pragma warning( pop )
#endif

}}} // namespace

#endif
