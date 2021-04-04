
#ifndef IGNIS_ITRANSPORTEXCEPTION_H
#define IGNIS_ITRANSPORTEXCEPTION_H
#define _THRIFT_TRANSPORT_TTRANSPORTEXCEPTION_H_ 1

//copy from <thrift/transport/TTransportException.h>

#include <string>
#include <thrift/Thrift.h>

namespace apache {
    namespace thrift {
        namespace transport {

/**
 * Class to encapsulate all the possible types of transport errors that may
 * occur in various transport systems. This provides a sort of generic
 * wrapper around the vague UNIX E_ error codes that lets a common code
 * base of error handling to be used for various types of transports, i.e.
 * pipes etc.
 *
 */
            class TTransportException : public apache::thrift::TException {
            public:
                /**
                 * Error codes for the various types of exceptions.
                 */
                enum TTransportExceptionType {
                    UNKNOWN = 0,
                    NOT_OPEN = 1,
                    TIMED_OUT = 2,
                    END_OF_FILE = 3,
                    INTERRUPTED = 4,
                    BAD_ARGS = 5,
                    CORRUPTED_DATA = 6,
                    INTERNAL_ERROR = 7,
                    CLIENT_DISCONNECT = 8
                };

                TTransportException() : apache::thrift::TException(), type_(UNKNOWN) {}

                TTransportException(TTransportExceptionType type) : apache::thrift::TException(), type_(type) {}

                TTransportException(const std::string& message)
                        : apache::thrift::TException(message), type_(UNKNOWN) {}

                TTransportException(TTransportExceptionType type, const std::string& message)
                        : apache::thrift::TException(message), type_(type) {}

                TTransportException(TTransportExceptionType type, const std::string& message, int errno_copy)
                        : apache::thrift::TException(message + ": " + TOutput::strerror_s(errno_copy)), type_(type) {}

                ~TTransportException() noexcept override = default;

                /**
                 * Returns an error code that provides information about the type of error
                 * that has occurred.
                 *
                 * @return Error code
                 */
                TTransportExceptionType getType() const noexcept { return type_; }

                const char* what() const noexcept override;

            protected:
                /** Just like strerror_r but returns a C++ string object. */
                std::string strerror_s(int errno_copy);

                /** Error code */
                TTransportExceptionType type_;
            };

/**
 * Legacy code in transport implementations have overflow issues
 * that need to be enforced.
 */
            template <typename To, typename From> To safe_numeric_cast(From i);

        }
    }
} // apache::thrift::transport


#endif//IGNIS_ITRANSPORTEXCEPTION_H
