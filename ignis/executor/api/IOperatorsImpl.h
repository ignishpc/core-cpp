
#ifndef IGNIS_IOPERATORSIMPL
#define IGNIS_IOPERATORSIMPL

#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>

#include <functional>
#include <boost/functional/hash/extensions.hpp>
#include "IReadIterator.h"
#include "IWriteIterator.h"


#ifndef IGNIS_DISABLE_ALL
namespace std {

#ifndef IGNIS_DISABLE_UNORDERER_MAP

    template<class K, class T, class C, class A>
    std::size_t hash_value(std::unordered_map<K, T, C, A> const &v) {
        return boost::hash_range(v.begin(), v.end());
    };

    template<class K, class T, class C, class A>
    struct hash<std::unordered_map<K, T, C, A>> {
        size_t operator()(std::unordered_map<K, T, C, A> const &v) const noexcept {
            return hash_value(v);
        }
    };

#endif


#ifndef IGNIS_DISABLE_UNORDERED_SET

    template<class K, class C, class A>
    std::size_t hash_value(std::unordered_set<K, C, A> const &v) {
        return boost::hash_range(v.begin(), v.end());
    };

    template<class K, class C, class A>
    struct hash<std::unordered_set<K, C, A>> {
        size_t operator()(std::unordered_set<K, C, A> const &v) const noexcept {
            return hash_value(v);
        }
    };

#endif


#ifndef IGNIS_DISABLE_PAIR

    template<class A, class B>
    struct hash<std::pair<A, B>> {
        std::size_t operator()(std::pair<A, B> const &v) const noexcept {
            return boost::hash_value(v);
        }
    };

#endif

#ifndef IGNIS_DISABLE_VECTOR

    template<class T, class A>
    struct hash<std::vector<T, A>> {
        std::size_t operator()(std::vector<T, A> const &v) const noexcept {
            return boost::hash_value(v);
        }
    };

#endif

#ifndef IGNIS_DISABLE_LIST

    template<class T, class A>
    struct hash<std::list<T, A>> {
        std::size_t operator()(std::list<T, A> const &v) const noexcept {
            return boost::hash_value(v);
        }
    };

#endif

#ifndef IGNIS_DISABLE_DEQUE

    template<class T, class A>
    struct hash<std::deque<T, A>> {
        std::size_t operator()(std::deque<T, A> const &v) const noexcept {
            return boost::hash_value(v);
        }
    };

#endif

#ifndef IGNIS_DISABLE_SET

    template<class K, class C, class A>
    struct hash<std::set<K, C, A>> {
        std::size_t operator()(std::set<K, C, A> const &v) const noexcept {
            return boost::hash_value(v);
        }
    };

#endif

#ifndef IGNIS_DISABLE_MULTISET

    template<class K, class C, class A>
    struct hash<std::multiset<K, C, A>> {
        std::size_t operator()(std::multiset<K, C, A> const &v) const noexcept {
            return boost::hash_value(v);
        }
    };

#endif

#ifndef IGNIS_DISABLE_MAP

    template<class K, class T, class C, class A>
    struct hash<std::map<K, T, C, A>> {
        std::size_t operator()(std::map<K, T, C, A> const &v) const noexcept {
            return boost::hash_value(v);
        }
    };

#endif

#ifndef IGNIS_DISABLE_MULTIMAP

    template<class K, class T, class C, class A>
    struct hash<std::multimap<K, T, C, A>> {
        std::size_t operator()(std::multimap<K, T, C, A> const &v) const noexcept {
            return boost::hash_value(v);
        }
    };

#endif

}
#endif
namespace std {

    template<class T>
    struct hash<ignis::executor::api::IReadIterator<T>> {
        std::size_t operator()(ignis::executor::api::IReadIterator<T> const &i) const noexcept {
            return hash_value(i);
        }
    };

    template<class T>
    struct hash<ignis::executor::api::IWriteIterator<T>> {
        std::size_t operator()(ignis::executor::api::IWriteIterator<T> const &i) const noexcept {
            return hash_value(i);
        }
    };
}
namespace ignis {
    namespace executor {
        namespace api {
            template<class T>
            inline bool operator==(const IReadIterator<T> &a, const IReadIterator<T> &b) {}

            template<class T>
            inline bool operator>(const IReadIterator<T> &a, const IReadIterator<T> &b) {}

            template<class T>
            inline bool operator<(const IReadIterator<T> &a, const IReadIterator<T> &b) {}

            template<class T>
            std::size_t hash_value(ignis::executor::api::IReadIterator<T> const &i) {}

            template<class T>
            inline bool operator==(const IWriteIterator<T> &a, const IWriteIterator<T> &b) {}

            template<class T>
            inline bool operator>(const IWriteIterator<T> &a, const IWriteIterator<T> &b) {}

            template<class T>
            inline bool operator<(const IWriteIterator<T> &a, const IWriteIterator<T> &b) {}

            template<class T>
            std::size_t hash_value(ignis::executor::api::IWriteIterator<T> const &i) {}
        }
    }
}

#endif
