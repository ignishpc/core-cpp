/**
 * Autogenerated by Thrift Compiler (0.11.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef IMapperModule_H
#define IMapperModule_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include "IMapperModule_types.h"

namespace ignis { namespace rpc { namespace executor {

#ifdef _MSC_VER
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class IMapperModuleIf {
 public:
  virtual ~IMapperModuleIf() {}
  virtual void _map(const  ::ignis::rpc::ISourceFunction& funct) = 0;
  virtual void flatmap(const  ::ignis::rpc::ISourceFunction& funct) = 0;
  virtual void filter(const  ::ignis::rpc::ISourceFunction& funct) = 0;
  virtual void streamingMap(const  ::ignis::rpc::ISourceFunction& funct, const bool ordered) = 0;
  virtual void streamingFlatmap(const  ::ignis::rpc::ISourceFunction& funct, const bool ordered) = 0;
  virtual void streamingFilter(const  ::ignis::rpc::ISourceFunction& funct, const bool ordered) = 0;
};

class IMapperModuleIfFactory {
 public:
  typedef IMapperModuleIf Handler;

  virtual ~IMapperModuleIfFactory() {}

  virtual IMapperModuleIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(IMapperModuleIf* /* handler */) = 0;
};

class IMapperModuleIfSingletonFactory : virtual public IMapperModuleIfFactory {
 public:
  IMapperModuleIfSingletonFactory(const ::apache::thrift::stdcxx::shared_ptr<IMapperModuleIf>& iface) : iface_(iface) {}
  virtual ~IMapperModuleIfSingletonFactory() {}

  virtual IMapperModuleIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(IMapperModuleIf* /* handler */) {}

 protected:
  ::apache::thrift::stdcxx::shared_ptr<IMapperModuleIf> iface_;
};

class IMapperModuleNull : virtual public IMapperModuleIf {
 public:
  virtual ~IMapperModuleNull() {}
  void _map(const  ::ignis::rpc::ISourceFunction& /* funct */) {
    return;
  }
  void flatmap(const  ::ignis::rpc::ISourceFunction& /* funct */) {
    return;
  }
  void filter(const  ::ignis::rpc::ISourceFunction& /* funct */) {
    return;
  }
  void streamingMap(const  ::ignis::rpc::ISourceFunction& /* funct */, const bool /* ordered */) {
    return;
  }
  void streamingFlatmap(const  ::ignis::rpc::ISourceFunction& /* funct */, const bool /* ordered */) {
    return;
  }
  void streamingFilter(const  ::ignis::rpc::ISourceFunction& /* funct */, const bool /* ordered */) {
    return;
  }
};

typedef struct _IMapperModule__map_args__isset {
  _IMapperModule__map_args__isset() : funct(false) {}
  bool funct :1;
} _IMapperModule__map_args__isset;

class IMapperModule__map_args {
 public:

  IMapperModule__map_args(const IMapperModule__map_args&);
  IMapperModule__map_args& operator=(const IMapperModule__map_args&);
  IMapperModule__map_args() {
  }

  virtual ~IMapperModule__map_args() throw();
   ::ignis::rpc::ISourceFunction funct;

  _IMapperModule__map_args__isset __isset;

  void __set_funct(const  ::ignis::rpc::ISourceFunction& val);

  bool operator == (const IMapperModule__map_args & rhs) const
  {
    if (!(funct == rhs.funct))
      return false;
    return true;
  }
  bool operator != (const IMapperModule__map_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const IMapperModule__map_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class IMapperModule__map_pargs {
 public:


  virtual ~IMapperModule__map_pargs() throw();
  const  ::ignis::rpc::ISourceFunction* funct;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _IMapperModule__map_result__isset {
  _IMapperModule__map_result__isset() : ex(false) {}
  bool ex :1;
} _IMapperModule__map_result__isset;

class IMapperModule__map_result {
 public:

  IMapperModule__map_result(const IMapperModule__map_result&);
  IMapperModule__map_result& operator=(const IMapperModule__map_result&);
  IMapperModule__map_result() {
  }

  virtual ~IMapperModule__map_result() throw();
   ::ignis::rpc::IRemoteException ex;

  _IMapperModule__map_result__isset __isset;

  void __set_ex(const  ::ignis::rpc::IRemoteException& val);

  bool operator == (const IMapperModule__map_result & rhs) const
  {
    if (!(ex == rhs.ex))
      return false;
    return true;
  }
  bool operator != (const IMapperModule__map_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const IMapperModule__map_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _IMapperModule__map_presult__isset {
  _IMapperModule__map_presult__isset() : ex(false) {}
  bool ex :1;
} _IMapperModule__map_presult__isset;

class IMapperModule__map_presult {
 public:


  virtual ~IMapperModule__map_presult() throw();
   ::ignis::rpc::IRemoteException ex;

  _IMapperModule__map_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _IMapperModule_flatmap_args__isset {
  _IMapperModule_flatmap_args__isset() : funct(false) {}
  bool funct :1;
} _IMapperModule_flatmap_args__isset;

class IMapperModule_flatmap_args {
 public:

  IMapperModule_flatmap_args(const IMapperModule_flatmap_args&);
  IMapperModule_flatmap_args& operator=(const IMapperModule_flatmap_args&);
  IMapperModule_flatmap_args() {
  }

  virtual ~IMapperModule_flatmap_args() throw();
   ::ignis::rpc::ISourceFunction funct;

  _IMapperModule_flatmap_args__isset __isset;

  void __set_funct(const  ::ignis::rpc::ISourceFunction& val);

  bool operator == (const IMapperModule_flatmap_args & rhs) const
  {
    if (!(funct == rhs.funct))
      return false;
    return true;
  }
  bool operator != (const IMapperModule_flatmap_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const IMapperModule_flatmap_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class IMapperModule_flatmap_pargs {
 public:


  virtual ~IMapperModule_flatmap_pargs() throw();
  const  ::ignis::rpc::ISourceFunction* funct;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _IMapperModule_flatmap_result__isset {
  _IMapperModule_flatmap_result__isset() : ex(false) {}
  bool ex :1;
} _IMapperModule_flatmap_result__isset;

class IMapperModule_flatmap_result {
 public:

  IMapperModule_flatmap_result(const IMapperModule_flatmap_result&);
  IMapperModule_flatmap_result& operator=(const IMapperModule_flatmap_result&);
  IMapperModule_flatmap_result() {
  }

  virtual ~IMapperModule_flatmap_result() throw();
   ::ignis::rpc::IRemoteException ex;

  _IMapperModule_flatmap_result__isset __isset;

  void __set_ex(const  ::ignis::rpc::IRemoteException& val);

  bool operator == (const IMapperModule_flatmap_result & rhs) const
  {
    if (!(ex == rhs.ex))
      return false;
    return true;
  }
  bool operator != (const IMapperModule_flatmap_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const IMapperModule_flatmap_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _IMapperModule_flatmap_presult__isset {
  _IMapperModule_flatmap_presult__isset() : ex(false) {}
  bool ex :1;
} _IMapperModule_flatmap_presult__isset;

class IMapperModule_flatmap_presult {
 public:


  virtual ~IMapperModule_flatmap_presult() throw();
   ::ignis::rpc::IRemoteException ex;

  _IMapperModule_flatmap_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _IMapperModule_filter_args__isset {
  _IMapperModule_filter_args__isset() : funct(false) {}
  bool funct :1;
} _IMapperModule_filter_args__isset;

class IMapperModule_filter_args {
 public:

  IMapperModule_filter_args(const IMapperModule_filter_args&);
  IMapperModule_filter_args& operator=(const IMapperModule_filter_args&);
  IMapperModule_filter_args() {
  }

  virtual ~IMapperModule_filter_args() throw();
   ::ignis::rpc::ISourceFunction funct;

  _IMapperModule_filter_args__isset __isset;

  void __set_funct(const  ::ignis::rpc::ISourceFunction& val);

  bool operator == (const IMapperModule_filter_args & rhs) const
  {
    if (!(funct == rhs.funct))
      return false;
    return true;
  }
  bool operator != (const IMapperModule_filter_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const IMapperModule_filter_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class IMapperModule_filter_pargs {
 public:


  virtual ~IMapperModule_filter_pargs() throw();
  const  ::ignis::rpc::ISourceFunction* funct;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _IMapperModule_filter_result__isset {
  _IMapperModule_filter_result__isset() : ex(false) {}
  bool ex :1;
} _IMapperModule_filter_result__isset;

class IMapperModule_filter_result {
 public:

  IMapperModule_filter_result(const IMapperModule_filter_result&);
  IMapperModule_filter_result& operator=(const IMapperModule_filter_result&);
  IMapperModule_filter_result() {
  }

  virtual ~IMapperModule_filter_result() throw();
   ::ignis::rpc::IRemoteException ex;

  _IMapperModule_filter_result__isset __isset;

  void __set_ex(const  ::ignis::rpc::IRemoteException& val);

  bool operator == (const IMapperModule_filter_result & rhs) const
  {
    if (!(ex == rhs.ex))
      return false;
    return true;
  }
  bool operator != (const IMapperModule_filter_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const IMapperModule_filter_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _IMapperModule_filter_presult__isset {
  _IMapperModule_filter_presult__isset() : ex(false) {}
  bool ex :1;
} _IMapperModule_filter_presult__isset;

class IMapperModule_filter_presult {
 public:


  virtual ~IMapperModule_filter_presult() throw();
   ::ignis::rpc::IRemoteException ex;

  _IMapperModule_filter_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _IMapperModule_streamingMap_args__isset {
  _IMapperModule_streamingMap_args__isset() : funct(false), ordered(false) {}
  bool funct :1;
  bool ordered :1;
} _IMapperModule_streamingMap_args__isset;

class IMapperModule_streamingMap_args {
 public:

  IMapperModule_streamingMap_args(const IMapperModule_streamingMap_args&);
  IMapperModule_streamingMap_args& operator=(const IMapperModule_streamingMap_args&);
  IMapperModule_streamingMap_args() : ordered(0) {
  }

  virtual ~IMapperModule_streamingMap_args() throw();
   ::ignis::rpc::ISourceFunction funct;
  bool ordered;

  _IMapperModule_streamingMap_args__isset __isset;

  void __set_funct(const  ::ignis::rpc::ISourceFunction& val);

  void __set_ordered(const bool val);

  bool operator == (const IMapperModule_streamingMap_args & rhs) const
  {
    if (!(funct == rhs.funct))
      return false;
    if (!(ordered == rhs.ordered))
      return false;
    return true;
  }
  bool operator != (const IMapperModule_streamingMap_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const IMapperModule_streamingMap_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class IMapperModule_streamingMap_pargs {
 public:


  virtual ~IMapperModule_streamingMap_pargs() throw();
  const  ::ignis::rpc::ISourceFunction* funct;
  const bool* ordered;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _IMapperModule_streamingMap_result__isset {
  _IMapperModule_streamingMap_result__isset() : ex(false) {}
  bool ex :1;
} _IMapperModule_streamingMap_result__isset;

class IMapperModule_streamingMap_result {
 public:

  IMapperModule_streamingMap_result(const IMapperModule_streamingMap_result&);
  IMapperModule_streamingMap_result& operator=(const IMapperModule_streamingMap_result&);
  IMapperModule_streamingMap_result() {
  }

  virtual ~IMapperModule_streamingMap_result() throw();
   ::ignis::rpc::IRemoteException ex;

  _IMapperModule_streamingMap_result__isset __isset;

  void __set_ex(const  ::ignis::rpc::IRemoteException& val);

  bool operator == (const IMapperModule_streamingMap_result & rhs) const
  {
    if (!(ex == rhs.ex))
      return false;
    return true;
  }
  bool operator != (const IMapperModule_streamingMap_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const IMapperModule_streamingMap_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _IMapperModule_streamingMap_presult__isset {
  _IMapperModule_streamingMap_presult__isset() : ex(false) {}
  bool ex :1;
} _IMapperModule_streamingMap_presult__isset;

class IMapperModule_streamingMap_presult {
 public:


  virtual ~IMapperModule_streamingMap_presult() throw();
   ::ignis::rpc::IRemoteException ex;

  _IMapperModule_streamingMap_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _IMapperModule_streamingFlatmap_args__isset {
  _IMapperModule_streamingFlatmap_args__isset() : funct(false), ordered(false) {}
  bool funct :1;
  bool ordered :1;
} _IMapperModule_streamingFlatmap_args__isset;

class IMapperModule_streamingFlatmap_args {
 public:

  IMapperModule_streamingFlatmap_args(const IMapperModule_streamingFlatmap_args&);
  IMapperModule_streamingFlatmap_args& operator=(const IMapperModule_streamingFlatmap_args&);
  IMapperModule_streamingFlatmap_args() : ordered(0) {
  }

  virtual ~IMapperModule_streamingFlatmap_args() throw();
   ::ignis::rpc::ISourceFunction funct;
  bool ordered;

  _IMapperModule_streamingFlatmap_args__isset __isset;

  void __set_funct(const  ::ignis::rpc::ISourceFunction& val);

  void __set_ordered(const bool val);

  bool operator == (const IMapperModule_streamingFlatmap_args & rhs) const
  {
    if (!(funct == rhs.funct))
      return false;
    if (!(ordered == rhs.ordered))
      return false;
    return true;
  }
  bool operator != (const IMapperModule_streamingFlatmap_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const IMapperModule_streamingFlatmap_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class IMapperModule_streamingFlatmap_pargs {
 public:


  virtual ~IMapperModule_streamingFlatmap_pargs() throw();
  const  ::ignis::rpc::ISourceFunction* funct;
  const bool* ordered;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _IMapperModule_streamingFlatmap_result__isset {
  _IMapperModule_streamingFlatmap_result__isset() : ex(false) {}
  bool ex :1;
} _IMapperModule_streamingFlatmap_result__isset;

class IMapperModule_streamingFlatmap_result {
 public:

  IMapperModule_streamingFlatmap_result(const IMapperModule_streamingFlatmap_result&);
  IMapperModule_streamingFlatmap_result& operator=(const IMapperModule_streamingFlatmap_result&);
  IMapperModule_streamingFlatmap_result() {
  }

  virtual ~IMapperModule_streamingFlatmap_result() throw();
   ::ignis::rpc::IRemoteException ex;

  _IMapperModule_streamingFlatmap_result__isset __isset;

  void __set_ex(const  ::ignis::rpc::IRemoteException& val);

  bool operator == (const IMapperModule_streamingFlatmap_result & rhs) const
  {
    if (!(ex == rhs.ex))
      return false;
    return true;
  }
  bool operator != (const IMapperModule_streamingFlatmap_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const IMapperModule_streamingFlatmap_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _IMapperModule_streamingFlatmap_presult__isset {
  _IMapperModule_streamingFlatmap_presult__isset() : ex(false) {}
  bool ex :1;
} _IMapperModule_streamingFlatmap_presult__isset;

class IMapperModule_streamingFlatmap_presult {
 public:


  virtual ~IMapperModule_streamingFlatmap_presult() throw();
   ::ignis::rpc::IRemoteException ex;

  _IMapperModule_streamingFlatmap_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _IMapperModule_streamingFilter_args__isset {
  _IMapperModule_streamingFilter_args__isset() : funct(false), ordered(false) {}
  bool funct :1;
  bool ordered :1;
} _IMapperModule_streamingFilter_args__isset;

class IMapperModule_streamingFilter_args {
 public:

  IMapperModule_streamingFilter_args(const IMapperModule_streamingFilter_args&);
  IMapperModule_streamingFilter_args& operator=(const IMapperModule_streamingFilter_args&);
  IMapperModule_streamingFilter_args() : ordered(0) {
  }

  virtual ~IMapperModule_streamingFilter_args() throw();
   ::ignis::rpc::ISourceFunction funct;
  bool ordered;

  _IMapperModule_streamingFilter_args__isset __isset;

  void __set_funct(const  ::ignis::rpc::ISourceFunction& val);

  void __set_ordered(const bool val);

  bool operator == (const IMapperModule_streamingFilter_args & rhs) const
  {
    if (!(funct == rhs.funct))
      return false;
    if (!(ordered == rhs.ordered))
      return false;
    return true;
  }
  bool operator != (const IMapperModule_streamingFilter_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const IMapperModule_streamingFilter_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class IMapperModule_streamingFilter_pargs {
 public:


  virtual ~IMapperModule_streamingFilter_pargs() throw();
  const  ::ignis::rpc::ISourceFunction* funct;
  const bool* ordered;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _IMapperModule_streamingFilter_result__isset {
  _IMapperModule_streamingFilter_result__isset() : ex(false) {}
  bool ex :1;
} _IMapperModule_streamingFilter_result__isset;

class IMapperModule_streamingFilter_result {
 public:

  IMapperModule_streamingFilter_result(const IMapperModule_streamingFilter_result&);
  IMapperModule_streamingFilter_result& operator=(const IMapperModule_streamingFilter_result&);
  IMapperModule_streamingFilter_result() {
  }

  virtual ~IMapperModule_streamingFilter_result() throw();
   ::ignis::rpc::IRemoteException ex;

  _IMapperModule_streamingFilter_result__isset __isset;

  void __set_ex(const  ::ignis::rpc::IRemoteException& val);

  bool operator == (const IMapperModule_streamingFilter_result & rhs) const
  {
    if (!(ex == rhs.ex))
      return false;
    return true;
  }
  bool operator != (const IMapperModule_streamingFilter_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const IMapperModule_streamingFilter_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _IMapperModule_streamingFilter_presult__isset {
  _IMapperModule_streamingFilter_presult__isset() : ex(false) {}
  bool ex :1;
} _IMapperModule_streamingFilter_presult__isset;

class IMapperModule_streamingFilter_presult {
 public:


  virtual ~IMapperModule_streamingFilter_presult() throw();
   ::ignis::rpc::IRemoteException ex;

  _IMapperModule_streamingFilter_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class IMapperModuleClient : virtual public IMapperModuleIf {
 public:
  IMapperModuleClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  IMapperModuleClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
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
  void _map(const  ::ignis::rpc::ISourceFunction& funct);
  void send__map(const  ::ignis::rpc::ISourceFunction& funct);
  void recv__map();
  void flatmap(const  ::ignis::rpc::ISourceFunction& funct);
  void send_flatmap(const  ::ignis::rpc::ISourceFunction& funct);
  void recv_flatmap();
  void filter(const  ::ignis::rpc::ISourceFunction& funct);
  void send_filter(const  ::ignis::rpc::ISourceFunction& funct);
  void recv_filter();
  void streamingMap(const  ::ignis::rpc::ISourceFunction& funct, const bool ordered);
  void send_streamingMap(const  ::ignis::rpc::ISourceFunction& funct, const bool ordered);
  void recv_streamingMap();
  void streamingFlatmap(const  ::ignis::rpc::ISourceFunction& funct, const bool ordered);
  void send_streamingFlatmap(const  ::ignis::rpc::ISourceFunction& funct, const bool ordered);
  void recv_streamingFlatmap();
  void streamingFilter(const  ::ignis::rpc::ISourceFunction& funct, const bool ordered);
  void send_streamingFilter(const  ::ignis::rpc::ISourceFunction& funct, const bool ordered);
  void recv_streamingFilter();
 protected:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class IMapperModuleProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  ::apache::thrift::stdcxx::shared_ptr<IMapperModuleIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (IMapperModuleProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process__map(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_flatmap(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_filter(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_streamingMap(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_streamingFlatmap(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_streamingFilter(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  IMapperModuleProcessor(::apache::thrift::stdcxx::shared_ptr<IMapperModuleIf> iface) :
    iface_(iface) {
    processMap_["_map"] = &IMapperModuleProcessor::process__map;
    processMap_["flatmap"] = &IMapperModuleProcessor::process_flatmap;
    processMap_["filter"] = &IMapperModuleProcessor::process_filter;
    processMap_["streamingMap"] = &IMapperModuleProcessor::process_streamingMap;
    processMap_["streamingFlatmap"] = &IMapperModuleProcessor::process_streamingFlatmap;
    processMap_["streamingFilter"] = &IMapperModuleProcessor::process_streamingFilter;
  }

  virtual ~IMapperModuleProcessor() {}
};

class IMapperModuleProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  IMapperModuleProcessorFactory(const ::apache::thrift::stdcxx::shared_ptr< IMapperModuleIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::apache::thrift::stdcxx::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::apache::thrift::stdcxx::shared_ptr< IMapperModuleIfFactory > handlerFactory_;
};

class IMapperModuleMultiface : virtual public IMapperModuleIf {
 public:
  IMapperModuleMultiface(std::vector<apache::thrift::stdcxx::shared_ptr<IMapperModuleIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~IMapperModuleMultiface() {}
 protected:
  std::vector<apache::thrift::stdcxx::shared_ptr<IMapperModuleIf> > ifaces_;
  IMapperModuleMultiface() {}
  void add(::apache::thrift::stdcxx::shared_ptr<IMapperModuleIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void _map(const  ::ignis::rpc::ISourceFunction& funct) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->_map(funct);
    }
    ifaces_[i]->_map(funct);
  }

  void flatmap(const  ::ignis::rpc::ISourceFunction& funct) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->flatmap(funct);
    }
    ifaces_[i]->flatmap(funct);
  }

  void filter(const  ::ignis::rpc::ISourceFunction& funct) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->filter(funct);
    }
    ifaces_[i]->filter(funct);
  }

  void streamingMap(const  ::ignis::rpc::ISourceFunction& funct, const bool ordered) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->streamingMap(funct, ordered);
    }
    ifaces_[i]->streamingMap(funct, ordered);
  }

  void streamingFlatmap(const  ::ignis::rpc::ISourceFunction& funct, const bool ordered) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->streamingFlatmap(funct, ordered);
    }
    ifaces_[i]->streamingFlatmap(funct, ordered);
  }

  void streamingFilter(const  ::ignis::rpc::ISourceFunction& funct, const bool ordered) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->streamingFilter(funct, ordered);
    }
    ifaces_[i]->streamingFilter(funct, ordered);
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class IMapperModuleConcurrentClient : virtual public IMapperModuleIf {
 public:
  IMapperModuleConcurrentClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  IMapperModuleConcurrentClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
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
  void _map(const  ::ignis::rpc::ISourceFunction& funct);
  int32_t send__map(const  ::ignis::rpc::ISourceFunction& funct);
  void recv__map(const int32_t seqid);
  void flatmap(const  ::ignis::rpc::ISourceFunction& funct);
  int32_t send_flatmap(const  ::ignis::rpc::ISourceFunction& funct);
  void recv_flatmap(const int32_t seqid);
  void filter(const  ::ignis::rpc::ISourceFunction& funct);
  int32_t send_filter(const  ::ignis::rpc::ISourceFunction& funct);
  void recv_filter(const int32_t seqid);
  void streamingMap(const  ::ignis::rpc::ISourceFunction& funct, const bool ordered);
  int32_t send_streamingMap(const  ::ignis::rpc::ISourceFunction& funct, const bool ordered);
  void recv_streamingMap(const int32_t seqid);
  void streamingFlatmap(const  ::ignis::rpc::ISourceFunction& funct, const bool ordered);
  int32_t send_streamingFlatmap(const  ::ignis::rpc::ISourceFunction& funct, const bool ordered);
  void recv_streamingFlatmap(const int32_t seqid);
  void streamingFilter(const  ::ignis::rpc::ISourceFunction& funct, const bool ordered);
  int32_t send_streamingFilter(const  ::ignis::rpc::ISourceFunction& funct, const bool ordered);
  void recv_streamingFilter(const int32_t seqid);
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
