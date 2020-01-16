/**
 * Autogenerated by Thrift Compiler (0.13.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "IBackendService.h"

namespace ignis { namespace rpc { namespace driver {


IBackendService_stop_args::~IBackendService_stop_args() noexcept {
}


uint32_t IBackendService_stop_args::read(::apache::thrift::protocol::TProtocol* iprot) {

  ::apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    xfer += iprot->skip(ftype);
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t IBackendService_stop_args::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  ::apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("IBackendService_stop_args");

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}


IBackendService_stop_pargs::~IBackendService_stop_pargs() noexcept {
}


uint32_t IBackendService_stop_pargs::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  ::apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("IBackendService_stop_pargs");

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}


IBackendService_stop_result::~IBackendService_stop_result() noexcept {
}


uint32_t IBackendService_stop_result::read(::apache::thrift::protocol::TProtocol* iprot) {

  ::apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRUCT) {
          xfer += this->ex.read(iprot);
          this->__isset.ex = true;
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

  return xfer;
}

uint32_t IBackendService_stop_result::write(::apache::thrift::protocol::TProtocol* oprot) const {

  uint32_t xfer = 0;

  xfer += oprot->writeStructBegin("IBackendService_stop_result");

  if (this->__isset.ex) {
    xfer += oprot->writeFieldBegin("ex", ::apache::thrift::protocol::T_STRUCT, 1);
    xfer += this->ex.write(oprot);
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}


IBackendService_stop_presult::~IBackendService_stop_presult() noexcept {
}


uint32_t IBackendService_stop_presult::read(::apache::thrift::protocol::TProtocol* iprot) {

  ::apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRUCT) {
          xfer += this->ex.read(iprot);
          this->__isset.ex = true;
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

  return xfer;
}

void IBackendServiceClient::stop()
{
  send_stop();
  recv_stop();
}

void IBackendServiceClient::send_stop()
{
  int32_t cseqid = 0;
  oprot_->writeMessageBegin("stop", ::apache::thrift::protocol::T_CALL, cseqid);

  IBackendService_stop_pargs args;
  args.write(oprot_);

  oprot_->writeMessageEnd();
  oprot_->getTransport()->writeEnd();
  oprot_->getTransport()->flush();
}

void IBackendServiceClient::recv_stop()
{

  int32_t rseqid = 0;
  std::string fname;
  ::apache::thrift::protocol::TMessageType mtype;

  iprot_->readMessageBegin(fname, mtype, rseqid);
  if (mtype == ::apache::thrift::protocol::T_EXCEPTION) {
    ::apache::thrift::TApplicationException x;
    x.read(iprot_);
    iprot_->readMessageEnd();
    iprot_->getTransport()->readEnd();
    throw x;
  }
  if (mtype != ::apache::thrift::protocol::T_REPLY) {
    iprot_->skip(::apache::thrift::protocol::T_STRUCT);
    iprot_->readMessageEnd();
    iprot_->getTransport()->readEnd();
  }
  if (fname.compare("stop") != 0) {
    iprot_->skip(::apache::thrift::protocol::T_STRUCT);
    iprot_->readMessageEnd();
    iprot_->getTransport()->readEnd();
  }
  IBackendService_stop_presult result;
  result.read(iprot_);
  iprot_->readMessageEnd();
  iprot_->getTransport()->readEnd();

  if (result.__isset.ex) {
    throw result.ex;
  }
  return;
}

bool IBackendServiceProcessor::dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext) {
  ProcessMap::iterator pfn;
  pfn = processMap_.find(fname);
  if (pfn == processMap_.end()) {
    iprot->skip(::apache::thrift::protocol::T_STRUCT);
    iprot->readMessageEnd();
    iprot->getTransport()->readEnd();
    ::apache::thrift::TApplicationException x(::apache::thrift::TApplicationException::UNKNOWN_METHOD, "Invalid method name: '"+fname+"'");
    oprot->writeMessageBegin(fname, ::apache::thrift::protocol::T_EXCEPTION, seqid);
    x.write(oprot);
    oprot->writeMessageEnd();
    oprot->getTransport()->writeEnd();
    oprot->getTransport()->flush();
    return true;
  }
  (this->*(pfn->second))(seqid, iprot, oprot, callContext);
  return true;
}

void IBackendServiceProcessor::process_stop(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext)
{
  void* ctx = NULL;
  if (this->eventHandler_.get() != NULL) {
    ctx = this->eventHandler_->getContext("IBackendService.stop", callContext);
  }
  ::apache::thrift::TProcessorContextFreer freer(this->eventHandler_.get(), ctx, "IBackendService.stop");

  if (this->eventHandler_.get() != NULL) {
    this->eventHandler_->preRead(ctx, "IBackendService.stop");
  }

  IBackendService_stop_args args;
  args.read(iprot);
  iprot->readMessageEnd();
  uint32_t bytes = iprot->getTransport()->readEnd();

  if (this->eventHandler_.get() != NULL) {
    this->eventHandler_->postRead(ctx, "IBackendService.stop", bytes);
  }

  IBackendService_stop_result result;
  try {
    iface_->stop();
  } catch ( ::ignis::rpc::driver::IDriverException &ex) {
    result.ex = ex;
    result.__isset.ex = true;
  } catch (const std::exception& e) {
    if (this->eventHandler_.get() != NULL) {
      this->eventHandler_->handlerError(ctx, "IBackendService.stop");
    }

    ::apache::thrift::TApplicationException x(e.what());
    oprot->writeMessageBegin("stop", ::apache::thrift::protocol::T_EXCEPTION, seqid);
    x.write(oprot);
    oprot->writeMessageEnd();
    oprot->getTransport()->writeEnd();
    oprot->getTransport()->flush();
    return;
  }

  if (this->eventHandler_.get() != NULL) {
    this->eventHandler_->preWrite(ctx, "IBackendService.stop");
  }

  oprot->writeMessageBegin("stop", ::apache::thrift::protocol::T_REPLY, seqid);
  result.write(oprot);
  oprot->writeMessageEnd();
  bytes = oprot->getTransport()->writeEnd();
  oprot->getTransport()->flush();

  if (this->eventHandler_.get() != NULL) {
    this->eventHandler_->postWrite(ctx, "IBackendService.stop", bytes);
  }
}

::std::shared_ptr< ::apache::thrift::TProcessor > IBackendServiceProcessorFactory::getProcessor(const ::apache::thrift::TConnectionInfo& connInfo) {
  ::apache::thrift::ReleaseHandler< IBackendServiceIfFactory > cleanup(handlerFactory_);
  ::std::shared_ptr< IBackendServiceIf > handler(handlerFactory_->getHandler(connInfo), cleanup);
  ::std::shared_ptr< ::apache::thrift::TProcessor > processor(new IBackendServiceProcessor(handler));
  return processor;
}

void IBackendServiceConcurrentClient::stop()
{
  int32_t seqid = send_stop();
  recv_stop(seqid);
}

int32_t IBackendServiceConcurrentClient::send_stop()
{
  int32_t cseqid = this->sync_->generateSeqId();
  ::apache::thrift::async::TConcurrentSendSentry sentry(this->sync_.get());
  oprot_->writeMessageBegin("stop", ::apache::thrift::protocol::T_CALL, cseqid);

  IBackendService_stop_pargs args;
  args.write(oprot_);

  oprot_->writeMessageEnd();
  oprot_->getTransport()->writeEnd();
  oprot_->getTransport()->flush();

  sentry.commit();
  return cseqid;
}

void IBackendServiceConcurrentClient::recv_stop(const int32_t seqid)
{

  int32_t rseqid = 0;
  std::string fname;
  ::apache::thrift::protocol::TMessageType mtype;

  // the read mutex gets dropped and reacquired as part of waitForWork()
  // The destructor of this sentry wakes up other clients
  ::apache::thrift::async::TConcurrentRecvSentry sentry(this->sync_.get(), seqid);

  while(true) {
    if(!this->sync_->getPending(fname, mtype, rseqid)) {
      iprot_->readMessageBegin(fname, mtype, rseqid);
    }
    if(seqid == rseqid) {
      if (mtype == ::apache::thrift::protocol::T_EXCEPTION) {
        ::apache::thrift::TApplicationException x;
        x.read(iprot_);
        iprot_->readMessageEnd();
        iprot_->getTransport()->readEnd();
        sentry.commit();
        throw x;
      }
      if (mtype != ::apache::thrift::protocol::T_REPLY) {
        iprot_->skip(::apache::thrift::protocol::T_STRUCT);
        iprot_->readMessageEnd();
        iprot_->getTransport()->readEnd();
      }
      if (fname.compare("stop") != 0) {
        iprot_->skip(::apache::thrift::protocol::T_STRUCT);
        iprot_->readMessageEnd();
        iprot_->getTransport()->readEnd();

        // in a bad state, don't commit
        using ::apache::thrift::protocol::TProtocolException;
        throw TProtocolException(TProtocolException::INVALID_DATA);
      }
      IBackendService_stop_presult result;
      result.read(iprot_);
      iprot_->readMessageEnd();
      iprot_->getTransport()->readEnd();

      if (result.__isset.ex) {
        sentry.commit();
        throw result.ex;
      }
      sentry.commit();
      return;
    }
    // seqid != rseqid
    this->sync_->updatePending(fname, mtype, rseqid);

    // this will temporarily unlock the readMutex, and let other clients get work done
    this->sync_->waitForWork(seqid);
  } // end while(true)
}

}}} // namespace

