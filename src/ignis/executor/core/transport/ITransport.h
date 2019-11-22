
#ifndef IGNIS_ITRANSPORT_H
#define IGNIS_ITRANSPORT_H

#include <thrift/transport/TTransport.h>
#include <thrift/transport/TVirtualTransport.h>
#include <thrift/transport/TSimpleFileTransport.h>

namespace ignis {
    namespace executor {
        namespace core {
            namespace transport {
                using ITransport = apache::thrift::transport::TTransport;
                using IFileTransport = apache::thrift::transport::TSimpleFileTransport;
            }
        }
    }
}

#endif

