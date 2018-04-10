
#ifndef EXECUTORCPP_ITUPLE_H
#define EXECUTORCPP_ITUPLE_H

#include <memory>
#include <utility>
#include "ITupleBase.h"

namespace ignis {
    namespace data {

        template<class T1, class T2>
        class ITuple : public ITupleBase<T1,T2>{
        public:

            constexpr ITuple() : ITupleBase<T1,T2>(new T1(), new T2()) {}

            template<class U1, class U2,
                    class = typename std::enable_if<std::__and_<std::is_convertible<const U1 &, T1>,
                            std::is_convertible<const U2 &, T2>>::value>::type>
            constexpr ITuple(const ITuple<U1, U2> &tuple) : ITupleBase<T1,T2>(new T1(tuple.first()), new T2(tuple.second())) {}

            template<class U1, class = typename
            std::enable_if<std::is_convertible<U1, T1>::value>::type>
            constexpr ITuple(U1 &&x, const T2 &y) : ITupleBase<T1,T2>(new T1(x), new T2(y)) {}

            template<class U2, class = typename
            std::enable_if<std::is_convertible<U2, T2>::value>::type>
            constexpr ITuple(const T1 &x, U2 &&y) : ITupleBase<T1,T2>(new T1(x), new T2(y)) {}

            template<class U1, class U2, class = typename
            std::enable_if<std::__and_<std::is_convertible<U1, T1>,
                    std::is_convertible<U2, T2>>::value>::type>
            constexpr ITuple(U1 &&x, U2 &&y) : ITupleBase<T1,T2>(new T1(x), new T2(y)) {}

            template<class U1, class U2, class = typename
            std::enable_if<std::__and_<std::is_convertible<U1, T1>,
                    std::is_convertible<U2, T2>>::value>::type>
            constexpr ITuple(const ITuple<U1, U2> &&tuple) : ITupleBase<T1,T2>(new T1(std::move(tuple.first())),
                                                             new T2(std::move(tuple.second()))) {}

            ITuple(const ITuple &tuple) : ITuple(tuple.first(), tuple.second()) {}

            ITuple(const ITuple &&tuple) : ITuple(std::move(tuple.first()), std::move(tuple.second())) {}

            ITuple &operator=(ITuple &tuple) {
                first() = tuple.first();
                second() = tuple.second();
                return *this;
            }

            ITuple &operator=(ITuple &&tuple)
            noexcept(std::__and_<std::is_nothrow_move_assignable<T1>,
                    std::is_nothrow_move_assignable<T2>>::value) {
                first() = tuple.first();
                second() = tuple.second();
                return *this;
            }

            template<class _U1, class _U2>
            ITuple &operator=(ITuple<_U1, _U2> &tuple) {
                first() = tuple.first();
                second() = tuple.second();
                return *this;
            }

            template<class _U1, class _U2>
            ITuple &operator=(ITuple<_U1, _U2> &&tuple) {
                first() = tuple.first();
                second() = tuple.second();
                return *this;
            }

            inline std::pair<T1, T2> asPair() const {
                return std::pair<T1 &, T2 &>(first(), second());
            };

            virtual ~ITuple() {
            }

            inline T1 &first() const {
                return *this->first_ptr;
            }

            inline T2 &second() const {
                return *this->second_ptr;
            }

        };

        template<class _T1, class _T2>
        inline bool
        operator==(const ITuple<_T1, _T2> &__x, const ITuple<_T1, _T2> &__y) {
            return (const ITupleBase<_T1, _T2> &)__x == (const ITupleBase<_T1, _T2> &)__y;
        }

        template<class _T1, class _T2>
        inline bool
        operator<(const ITuple<_T1, _T2> &__x, const ITuple<_T1, _T2> &__y) {
            return (const ITupleBase<_T1, _T2> &)__x < (const ITupleBase<_T1, _T2> &)__y;
        }

        template<class _T1, class _T2>
        inline bool
        operator!=(const ITuple<_T1, _T2> &__x, const ITuple<_T1, _T2> &__y) { return !(__x == __y); }

        template<class _T1, class _T2>
        inline bool
        operator>(const ITuple<_T1, _T2> &__x, const ITuple<_T1, _T2> &__y) { return __y < __x; }

        template<class _T1, class _T2>
        inline bool
        operator<=(const ITuple<_T1, _T2> &__x, const ITuple<_T1, _T2> &__y) { return !(__y < __x); }

        template<class _T1, class _T2>
        inline bool
        operator>=(const ITuple<_T1, _T2> &__x, const ITuple<_T1, _T2> &__y) { return !(__x < __y); }

    }
}

#endif
