
#ifndef IGNIS_IDRIVEREXCEPTION_H
#define IGNIS_IDRIVEREXCEPTION_H

#include <ostream>
#include <stdexcept>

namespace ignis {
    namespace driver {
        namespace api {
            class IDriverException : public std::runtime_error {
            public:
                IDriverException(const std::string &message);

                IDriverException(const std::string &message, const std::string &cause);

                bool hasCause();

                const std::string &getCause();

                std::ostream &operator<<(std::ostream &stream);

                virtual ~IDriverException();

            private:
                std::string cause;
            };

            inline std::ostream &operator<<(std::ostream &stream, IDriverException &ex) {
                ex.operator<<(stream);
                return stream;
            }
        }// namespace api
    }    // namespace driver
}// namespace ignis

#endif
