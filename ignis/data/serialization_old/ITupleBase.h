
#ifndef EXECUTORCPP_ITUPLEBASE_H
#define EXECUTORCPP_ITUPLEBASE_H

#include <memory>

namespace ignis {
    namespace data {

        template<class T1, class T2>
        class ITupleBase {
        protected:
            constexpr ITupleBase(T1 *first_ptr, T2 *second_ptr, bool handle_ptr = true) : first_ptr(first_ptr),
                                                                                          second_ptr(second_ptr),
                                                                                          handle_ptr(handle_ptr){}
            bool handle_ptr;
            T1 *first_ptr;
            T2 *second_ptr;

        public:
            constexpr ITupleBase(): first_ptr(NULL),
                                    second_ptr(NULL),
                                    handle_ptr(false){}

            constexpr ITupleBase(const ITupleBase &other) : ITupleBase(std::move(other)) {}

            ITupleBase(const ITupleBase &&other) {
                first_ptr = other.first_ptr;
                second_ptr = other.second_ptr;
                ((ITupleBase &)other).first_ptr = NULL;
                ((ITupleBase &)other).second_ptr = NULL;
            }

            template<class _U1, class _U2>
            ITupleBase &operator=(ITupleBase<_U1, _U2> &other) {
                first_ptr = other.first_ptr;
                second_ptr = other.second_ptr;
                other.first_ptr = NULL;
                other.second_ptr = NULL;
                return *this;
            }

            template<class _U1, class _U2>
            ITupleBase &operator=(ITupleBase<_U1, _U2> &&other) {
                first_ptr = other.first_ptr;
                second_ptr = other.second_ptr;
                other.first_ptr = NULL;
                other.second_ptr = NULL;
                return *this;
            }

            virtual ~ITupleBase() {
                if(handle_ptr) {
                    if (first_ptr != NULL)
                        delete first_ptr;
                    if (second_ptr != NULL)
                        delete second_ptr;
                }
            }

            inline bool operator==(const ITupleBase &other) const {
                return *first_ptr == *other.first_ptr && *second_ptr == *other.second_ptr;
            }

            inline bool operator<(const ITupleBase &other) const {
                return *first_ptr < *other.first_ptr
                       || (!(*other.first_ptr < *first_ptr) && *second_ptr < *other.second_ptr);
            }

        };

        template<class _T1, class _T2>
        inline bool
        operator==(const ITupleBase<_T1, _T2> &__x, const ITupleBase<_T1, _T2> &__y) {
            return __x.operator==(__y);
        }

        template<class _T1, class _T2>
        inline bool
        operator<(const ITupleBase<_T1, _T2> &__x, const ITupleBase<_T1, _T2> &__y) {
            return __x.operator<(__y);
        }

        template<class _T1, class _T2>
        inline bool
        operator!=(const ITupleBase<_T1, _T2> &__x, const ITupleBase<_T1, _T2> &__y) { return !(__x == __y); }

        template<class _T1, class _T2>
        inline bool
        operator>(const ITupleBase<_T1, _T2> &__x, const ITupleBase<_T1, _T2> &__y) { return __y < __x; }

        template<class _T1, class _T2>
        inline bool
        operator<=(const ITupleBase<_T1, _T2> &__x, const ITupleBase<_T1, _T2> &__y) { return !(__y < __x); }

        template<class _T1, class _T2>
        inline bool
        operator>=(const ITupleBase<_T1, _T2> &__x, const ITupleBase<_T1, _T2> &__y) { return !(__x < __y); }
    }
}

#endif
