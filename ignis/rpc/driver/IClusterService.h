/**
 * Autogenerated by Thrift Compiler (0.11.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef IClusterService_H
#define IClusterService_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include "IClusterService_types.h"

namespace ignis { namespace rpc { namespace driver {

#ifdef _MSC_VER
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class IClusterServiceIf {
 public:
  virtual ~IClusterServiceIf() {}
  virtual int64_t newInstance(const int64_t properties) = 0;
  virtual int32_t sendFiles(const int64_t cluster, const std::string& source, const std::string& target) = 0;
  virtual int32_t sendCompressedFile(const int64_t cluster, const std::string& source, const std::string& target) = 0;
  virtual void keep(const int64_t cluster) = 0;
};

class IClusterServiceIfFactory {
 public:
  typedef IClusterServiceIf Handler;

  virtual ~IClusterServiceIfFactory() {}

  virtual IClusterServiceIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(IClusterServiceIf* /* handler */) = 0;
};

class IClusterServiceIfSingletonFactory : virtual public IClusterServiceIfFactory {
 public:
  IClusterServiceIfSingletonFactory(const ::apache::thrift::stdcxx::shared_ptr<IClusterServiceIf>& iface) : iface_(iface) {}
  virtual ~IClusterServiceIfSingletonFactory() {}

  virtual IClusterServiceIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(IClusterServiceIf* /* handler */) {}

 protected:
  ::apache::thrift::stdcxx::shared_ptr<IClusterServiceIf> iface_;
};

class IClusterServiceNull : virtual public IClusterServiceIf {
 public:
  virtual ~IClusterServiceNull() {}
  int64_t newInstance(const int64_t /* properties */) {
    int64_t _return = 0;
    return _return;
  }
  int32_t sendFiles(const int64_t /* cluster */, const std::string& /* source */, const std::string& /* target */) {
    int32_t _return = 0;
    return _return;
  }
  int32_t sendCompressedFile(const int64_t /* cluster */, const std::string& /* source */, const std::string& /* target */) {
    int32_t _return = 0;
    return _return;
  }
  void keep(const int64_t /* cluster */) {
    return;
  }
};

typedef struct _IClusterService_newInstance_args__isset {
  _IClusterService_newInstance_args__isset() : properties(false) {}
  bool properties :1;
} _IClusterService_newInstance_args__isset;

class IClusterService_newInstance_args {
 public:

  IClusterService_newInstance_args(const IClusterService_newInstance_args&);
  IClusterService_newInstance_args& operator=(const IClusterService_newInstance_args&);
  IClusterService_newInstance_args() : properties(0) {
  }

  virtual ~IClusterService_newInstance_args() throw();
  int64_t properties;

  _IClusterService_newInstance_args__isset __isset;

  void __set_properties(const int64_t val);

  bool operator == (const IClusterService_newInstance_args & rhs) const
  {
    if (!(properties == rhs.properties))
      return false;
    return true;
  }
  bool operator != (const IClusterService_newInstance_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const IClusterService_newInstance_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class IClusterService_newInstance_pargs {
 public:


  virtual ~IClusterService_newInstance_pargs() throw();
  const int64_t* properties;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _IClusterService_newInstance_result__isset {
  _IClusterService_newInstance_result__isset() : success(false), ex(false) {}
  bool success :1;
  bool ex :1;
} _IClusterService_newInstance_result__isset;

class IClusterService_newInstance_result {
 public:

  IClusterService_newInstance_result(const IClusterService_newInstance_result&);
  IClusterService_newInstance_result& operator=(const IClusterService_newInstance_result&);
  IClusterService_newInstance_result() : success(0) {
  }

  virtual ~IClusterService_newInstance_result() throw();
  int64_t success;
   ::ignis::rpc::IRemoteException ex;

  _IClusterService_newInstance_result__isset __isset;

  void __set_success(const int64_t val);

  void __set_ex(const  ::ignis::rpc::IRemoteException& val);

  bool operator == (const IClusterService_newInstance_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    if (!(ex == rhs.ex))
      return false;
    return true;
  }
  bool operator != (const IClusterService_newInstance_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const IClusterService_newInstance_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _IClusterService_newInstance_presult__isset {
  _IClusterService_newInstance_presult__isset() : success(false), ex(false) {}
  bool success :1;
  bool ex :1;
} _IClusterService_newInstance_presult__isset;

class IClusterService_newInstance_presult {
 public:


  virtual ~IClusterService_newInstance_presult() throw();
  int64_t* success;
   ::ignis::rpc::IRemoteException ex;

  _IClusterService_newInstance_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _IClusterService_sendFiles_args__isset {
  _IClusterService_sendFiles_args__isset() : cluster(false), source(false), target(false) {}
  bool cluster :1;
  bool source :1;
  bool target :1;
} _IClusterService_sendFiles_args__isset;

class IClusterService_sendFiles_args {
 public:

  IClusterService_sendFiles_args(const IClusterService_sendFiles_args&);
  IClusterService_sendFiles_args& operator=(const IClusterService_sendFiles_args&);
  IClusterService_sendFiles_args() : cluster(0), source(), target() {
  }

  virtual ~IClusterService_sendFiles_args() throw();
  int64_t cluster;
  std::string source;
  std::string target;

  _IClusterService_sendFiles_args__isset __isset;

  void __set_cluster(const int64_t val);

  void __set_source(const std::string& val);

  void __set_target(const std::string& val);

  bool operator == (const IClusterService_sendFiles_args & rhs) const
  {
    if (!(cluster == rhs.cluster))
      return false;
    if (!(source == rhs.source))
      return false;
    if (!(target == rhs.target))
      return false;
    return true;
  }
  bool operator != (const IClusterService_sendFiles_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const IClusterService_sendFiles_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class IClusterService_sendFiles_pargs {
 public:


  virtual ~IClusterService_sendFiles_pargs() throw();
  const int64_t* cluster;
  const std::string* source;
  const std::string* target;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _IClusterService_sendFiles_result__isset {
  _IClusterService_sendFiles_result__isset() : success(false), ex(false) {}
  bool success :1;
  bool ex :1;
} _IClusterService_sendFiles_result__isset;

class IClusterService_sendFiles_result {
 public:

  IClusterService_sendFiles_result(const IClusterService_sendFiles_result&);
  IClusterService_sendFiles_result& operator=(const IClusterService_sendFiles_result&);
  IClusterService_sendFiles_result() : success(0) {
  }

  virtual ~IClusterService_sendFiles_result() throw();
  int32_t success;
   ::ignis::rpc::IRemoteException ex;

  _IClusterService_sendFiles_result__isset __isset;

  void __set_success(const int32_t val);

  void __set_ex(const  ::ignis::rpc::IRemoteException& val);

  bool operator == (const IClusterService_sendFiles_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    if (!(ex == rhs.ex))
      return false;
    return true;
  }
  bool operator != (const IClusterService_sendFiles_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const IClusterService_sendFiles_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _IClusterService_sendFiles_presult__isset {
  _IClusterService_sendFiles_presult__isset() : success(false), ex(false) {}
  bool success :1;
  bool ex :1;
} _IClusterService_sendFiles_presult__isset;

class IClusterService_sendFiles_presult {
 public:


  virtual ~IClusterService_sendFiles_presult() throw();
  int32_t* success;
   ::ignis::rpc::IRemoteException ex;

  _IClusterService_sendFiles_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _IClusterService_sendCompressedFile_args__isset {
  _IClusterService_sendCompressedFile_args__isset() : cluster(false), source(false), target(false) {}
  bool cluster :1;
  bool source :1;
  bool target :1;
} _IClusterService_sendCompressedFile_args__isset;

class IClusterService_sendCompressedFile_args {
 public:

  IClusterService_sendCompressedFile_args(const IClusterService_sendCompressedFile_args&);
  IClusterService_sendCompressedFile_args& operator=(const IClusterService_sendCompressedFile_args&);
  IClusterService_sendCompressedFile_args() : cluster(0), source(), target() {
  }

  virtual ~IClusterService_sendCompressedFile_args() throw();
  int64_t cluster;
  std::string source;
  std::string target;

  _IClusterService_sendCompressedFile_args__isset __isset;

  void __set_cluster(const int64_t val);

  void __set_source(const std::string& val);

  void __set_target(const std::string& val);

  bool operator == (const IClusterService_sendCompressedFile_args & rhs) const
  {
    if (!(cluster == rhs.cluster))
      return false;
    if (!(source == rhs.source))
      return false;
    if (!(target == rhs.target))
      return false;
    return true;
  }
  bool operator != (const IClusterService_sendCompressedFile_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const IClusterService_sendCompressedFile_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class IClusterService_sendCompressedFile_pargs {
 public:


  virtual ~IClusterService_sendCompressedFile_pargs() throw();
  const int64_t* cluster;
  const std::string* source;
  const std::string* target;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _IClusterService_sendCompressedFile_result__isset {
  _IClusterService_sendCompressedFile_result__isset() : success(false), ex(false) {}
  bool success :1;
  bool ex :1;
} _IClusterService_sendCompressedFile_result__isset;

class IClusterService_sendCompressedFile_result {
 public:

  IClusterService_sendCompressedFile_result(const IClusterService_sendCompressedFile_result&);
  IClusterService_sendCompressedFile_result& operator=(const IClusterService_sendCompressedFile_result&);
  IClusterService_sendCompressedFile_result() : success(0) {
  }

  virtual ~IClusterService_sendCompressedFile_result() throw();
  int32_t success;
   ::ignis::rpc::IRemoteException ex;

  _IClusterService_sendCompressedFile_result__isset __isset;

  void __set_success(const int32_t val);

  void __set_ex(const  ::ignis::rpc::IRemoteException& val);

  bool operator == (const IClusterService_sendCompressedFile_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    if (!(ex == rhs.ex))
      return false;
    return true;
  }
  bool operator != (const IClusterService_sendCompressedFile_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const IClusterService_sendCompressedFile_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _IClusterService_sendCompressedFile_presult__isset {
  _IClusterService_sendCompressedFile_presult__isset() : success(false), ex(false) {}
  bool success :1;
  bool ex :1;
} _IClusterService_sendCompressedFile_presult__isset;

class IClusterService_sendCompressedFile_presult {
 public:


  virtual ~IClusterService_sendCompressedFile_presult() throw();
  int32_t* success;
   ::ignis::rpc::IRemoteException ex;

  _IClusterService_sendCompressedFile_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _IClusterService_keep_args__isset {
  _IClusterService_keep_args__isset() : cluster(false) {}
  bool cluster :1;
} _IClusterService_keep_args__isset;

class IClusterService_keep_args {
 public:

  IClusterService_keep_args(const IClusterService_keep_args&);
  IClusterService_keep_args& operator=(const IClusterService_keep_args&);
  IClusterService_keep_args() : cluster(0) {
  }

  virtual ~IClusterService_keep_args() throw();
  int64_t cluster;

  _IClusterService_keep_args__isset __isset;

  void __set_cluster(const int64_t val);

  bool operator == (const IClusterService_keep_args & rhs) const
  {
    if (!(cluster == rhs.cluster))
      return false;
    return true;
  }
  bool operator != (const IClusterService_keep_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const IClusterService_keep_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class IClusterService_keep_pargs {
 public:


  virtual ~IClusterService_keep_pargs() throw();
  const int64_t* cluster;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _IClusterService_keep_result__isset {
  _IClusterService_keep_result__isset() : ex(false) {}
  bool ex :1;
} _IClusterService_keep_result__isset;

class IClusterService_keep_result {
 public:

  IClusterService_keep_result(const IClusterService_keep_result&);
  IClusterService_keep_result& operator=(const IClusterService_keep_result&);
  IClusterService_keep_result() {
  }

  virtual ~IClusterService_keep_result() throw();
   ::ignis::rpc::IRemoteException ex;

  _IClusterService_keep_result__isset __isset;

  void __set_ex(const  ::ignis::rpc::IRemoteException& val);

  bool operator == (const IClusterService_keep_result & rhs) const
  {
    if (!(ex == rhs.ex))
      return false;
    return true;
  }
  bool operator != (const IClusterService_keep_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const IClusterService_keep_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _IClusterService_keep_presult__isset {
  _IClusterService_keep_presult__isset() : ex(false) {}
  bool ex :1;
} _IClusterService_keep_presult__isset;

class IClusterService_keep_presult {
 public:


  virtual ~IClusterService_keep_presult() throw();
   ::ignis::rpc::IRemoteException ex;

  _IClusterService_keep_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class IClusterServiceClient : virtual public IClusterServiceIf {
 public:
  IClusterServiceClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  IClusterServiceClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
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
  int64_t newInstance(const int64_t properties);
  void send_newInstance(const int64_t properties);
  int64_t recv_newInstance();
  int32_t sendFiles(const int64_t cluster, const std::string& source, const std::string& target);
  void send_sendFiles(const int64_t cluster, const std::string& source, const std::string& target);
  int32_t recv_sendFiles();
  int32_t sendCompressedFile(const int64_t cluster, const std::string& source, const std::string& target);
  void send_sendCompressedFile(const int64_t cluster, const std::string& source, const std::string& target);
  int32_t recv_sendCompressedFile();
  void keep(const int64_t cluster);
  void send_keep(const int64_t cluster);
  void recv_keep();
 protected:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class IClusterServiceProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  ::apache::thrift::stdcxx::shared_ptr<IClusterServiceIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (IClusterServiceProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_newInstance(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_sendFiles(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_sendCompressedFile(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_keep(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  IClusterServiceProcessor(::apache::thrift::stdcxx::shared_ptr<IClusterServiceIf> iface) :
    iface_(iface) {
    processMap_["newInstance"] = &IClusterServiceProcessor::process_newInstance;
    processMap_["sendFiles"] = &IClusterServiceProcessor::process_sendFiles;
    processMap_["sendCompressedFile"] = &IClusterServiceProcessor::process_sendCompressedFile;
    processMap_["keep"] = &IClusterServiceProcessor::process_keep;
  }

  virtual ~IClusterServiceProcessor() {}
};

class IClusterServiceProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  IClusterServiceProcessorFactory(const ::apache::thrift::stdcxx::shared_ptr< IClusterServiceIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::apache::thrift::stdcxx::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::apache::thrift::stdcxx::shared_ptr< IClusterServiceIfFactory > handlerFactory_;
};

class IClusterServiceMultiface : virtual public IClusterServiceIf {
 public:
  IClusterServiceMultiface(std::vector<apache::thrift::stdcxx::shared_ptr<IClusterServiceIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~IClusterServiceMultiface() {}
 protected:
  std::vector<apache::thrift::stdcxx::shared_ptr<IClusterServiceIf> > ifaces_;
  IClusterServiceMultiface() {}
  void add(::apache::thrift::stdcxx::shared_ptr<IClusterServiceIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  int64_t newInstance(const int64_t properties) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->newInstance(properties);
    }
    return ifaces_[i]->newInstance(properties);
  }

  int32_t sendFiles(const int64_t cluster, const std::string& source, const std::string& target) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->sendFiles(cluster, source, target);
    }
    return ifaces_[i]->sendFiles(cluster, source, target);
  }

  int32_t sendCompressedFile(const int64_t cluster, const std::string& source, const std::string& target) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->sendCompressedFile(cluster, source, target);
    }
    return ifaces_[i]->sendCompressedFile(cluster, source, target);
  }

  void keep(const int64_t cluster) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->keep(cluster);
    }
    ifaces_[i]->keep(cluster);
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class IClusterServiceConcurrentClient : virtual public IClusterServiceIf {
 public:
  IClusterServiceConcurrentClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  IClusterServiceConcurrentClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
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
  int64_t newInstance(const int64_t properties);
  int32_t send_newInstance(const int64_t properties);
  int64_t recv_newInstance(const int32_t seqid);
  int32_t sendFiles(const int64_t cluster, const std::string& source, const std::string& target);
  int32_t send_sendFiles(const int64_t cluster, const std::string& source, const std::string& target);
  int32_t recv_sendFiles(const int32_t seqid);
  int32_t sendCompressedFile(const int64_t cluster, const std::string& source, const std::string& target);
  int32_t send_sendCompressedFile(const int64_t cluster, const std::string& source, const std::string& target);
  int32_t recv_sendCompressedFile(const int32_t seqid);
  void keep(const int64_t cluster);
  int32_t send_keep(const int64_t cluster);
  void recv_keep(const int32_t seqid);
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
