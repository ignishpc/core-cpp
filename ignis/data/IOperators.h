#ifndef IGNIS_OPERATORS_H
#define IGNIS_OPERATORS_H

#include <functional>
#include <memory>

namespace ignis {
    namespace data {


        template<typename T>
        class IOperator {
        public:

            virtual inline bool equals(const T &a, const T &b) {
                return a == b;
            }

            virtual bool notEquals(const T &a, const T &b) {
                return a != b;
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

            virtual std::shared_ptr<IOperator<T>> shared() const{
                return std::make_shared<IOperator<T>>();
            }

        };


    }
}

#endif
