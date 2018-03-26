
#ifndef EXECUTORCPP_IGNISEXECUTOREXCEPTION_H
#define EXECUTORCPP_IGNISEXECUTOREXCEPTION_H

#define  BOOST_STACKTRACE_USE_ADDR2LINE

#include <stdexcept>
#include <boost/stacktrace.hpp>
#include "../../data/RTTInfo.h"

namespace ignis {
    namespace executor {
        namespace api {

            class IgnisExecutorException : public std::runtime_error {

            public:

                IgnisExecutorException(const std::string &message);

                IgnisExecutorException(const std::string &message, IgnisExecutorException &cause);

                IgnisExecutorException(const char *message);

                IgnisExecutorException(const char *message, IgnisExecutorException &cause);

                virtual ~IgnisExecutorException();

                virtual bool hasCause();

                virtual IgnisExecutorException &getCause();

                virtual void printStackTrace(std::ostream &stream);

                virtual std::string getStacktrace();

                virtual void print(std::ostream &stream);

                virtual std::string toString();

                virtual void setCause(std::shared_ptr<IgnisExecutorException> &cause);

                friend std::ostream &operator<<(std::ostream &stream, IgnisExecutorException &ex) {
                    ex.print(stream);
                }

            protected:

                template<typename C = IgnisExecutorException>
                void setExceptionName(){
                    class_name = data::RTTInfo(typeid(C)).getStandardName();
                }

            private:
                std::string class_name;
                std::shared_ptr<IgnisExecutorException> cause;
                boost::stacktrace::stacktrace stacktrace;
            };
        }
    }
}

#endif
