
#ifndef IGNIS_IEXCEPTION_H
#define IGNIS_IEXCEPTION_H

#define  BOOST_STACKTRACE_USE_ADDR2LINE

#include <stdexcept>
#include <boost/stacktrace.hpp>
#include "../data/RTTInfo.h"

namespace ignis {
    namespace exceptions {

        class IException : public std::runtime_error {

        public:

            IException(const std::string &message);

            IException(const std::string &message, const IException &cause);

            IException(const char *message);

            IException(const char *message, const IException &cause);

            virtual ~IException();

            virtual bool hasCause() const;

            virtual IException &getCause();

            virtual void printStackTrace(std::ostream &stream) const;

            virtual std::string getStacktrace() const;

            virtual void print(std::ostream &stream) const;

            virtual std::string toString() const;

            virtual void setCause(std::shared_ptr<IException> &cause);

            virtual std::ostream &operator<<(std::ostream &stream) const;

        protected:

            template<typename C>
            void setExceptionName(C* c) {
                class_name = data::RTTInfo(typeid(C)).getStandardName();
            }

        private:
            std::string class_name;
            std::shared_ptr<IException> cause;
            boost::stacktrace::stacktrace stacktrace;
        };

        inline std::ostream &operator<<(std::ostream &stream, const IException &ex) {
            ex.operator<<(stream);
        }
    }
}


#endif
