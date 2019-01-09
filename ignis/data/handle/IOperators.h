#ifndef IGNIS_IOPERATORS_H
#define IGNIS_IOPERATORS_H

#include <functional>
#include <memory>
#include "IPrinter.h"
#include <sstream>

namespace ignis {
    namespace data {
        namespace handle {

            template<typename T>
            class IOperator {
            public:

                virtual inline bool equals(const T &a, const T &b) {
                    return a == b;
                }

                virtual inline bool less(const T &a, const T &b) {
                    return a < b;
                }

                virtual bool greater(const T &a, const T &b) {
                    return a > b;
                }

                virtual size_t hash(const T &a) {
                    return std::hash<T>()(a);
                }

                virtual std::string toString(const T &a) {
                    std::stringstream ss;
                    IPrinter<T>().operator()(a, ss);
                    return ss.str();
                }

            };
            
        }
    }
}
#endif
