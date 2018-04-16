
#ifndef IGNIS_IGNISEXCEPTION_H
#define IGNIS_IGNISEXCEPTION_H

#include <stdexcept>
#include <ostream>

namespace ignis {
    namespace driver {
        namespace api {
            class IgnisException : public std::runtime_error {
            public:
                IgnisException(const std::string &message, const std::string &stacktrace);

                std::string &getStacktrace();

                void print(std::ostream &stream);

                std::ostream &operator<<(std::ostream &stream) {
                    print(stream);
                }

                virtual ~IgnisException();

            private:
                std::string stacktrace;
            };

            inline std::ostream &operator<<(std::ostream &stream, IgnisException &ex) {
                ex.print(stream);
            }
        }
    }
}

#endif
