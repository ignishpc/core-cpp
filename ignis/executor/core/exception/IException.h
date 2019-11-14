
#ifndef IGNIS_IEXCEPTION_H
#define IGNIS_IEXCEPTION_H

#include <stdexcept>
#include <memory>
#include "executor/core/RTTInfo.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace exception {

                class IException : public std::runtime_error {

                public:

                    IException(const IException &other);

                    IException(const std::string &message);

                    IException(const std::string &message, const IException &cause);

                    IException(const std::exception &cause);

                    virtual ~IException();

                    virtual bool hasCause() const;

                    virtual const IException &getCause() const;

                    virtual void printStackTrace(std::ostream &stream) const;

                    virtual std::string getStacktrace() const;

                    virtual void print(std::ostream &stream) const;

                    virtual std::string toString() const;

                    virtual std::ostream &operator<<(std::ostream &stream) const;

                protected:

                    virtual std::string exceptionName() const;

                    template<typename Ex>
                    std::string parseName() const { return RTTInfo::from<Ex>().getStandardName(); }

                private:
                    const std::shared_ptr<IException> cause;
                    const std::shared_ptr<void> stacktrace;
                };

                inline std::ostream &operator<<(std::ostream &stream, const IException &ex) {
                    ex.operator<<(stream);
                }
            }
        }
    }
}

#endif

