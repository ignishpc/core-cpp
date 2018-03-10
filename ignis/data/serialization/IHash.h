
#ifndef UNTITLED_IHASH_H
#define UNTITLED_IHASH_H

#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>

#include <functional>
#include <boost/functional/hash/extensions.hpp>

namespace std {

    template<class K, class T, class C, class A>
    std::size_t hash_value(std::unordered_map<K, T, C, A> const &v) {
        return boost::hash_range(v.begin(), v.end());
    };

    template<class K, class C, class A>
    std::size_t hash_value(std::unordered_set<K, C, A> const &v) {
        return boost::hash_range(v.begin(), v.end());
    };

    template<class K, class T, class C, class A>
    struct hash<std::unordered_map<K, T, C, A>> {
        size_t operator()(std::unordered_map<K, T, C, A> const &v) const noexcept {
            return hash_value(v);
        }
    };

    template<class K, class C, class A>
    struct hash<std::unordered_set<K, C, A>> {
        size_t operator()(std::unordered_set<K, C, A> const &v) const noexcept {
            return hash_value(v);
        }
    };

    template<class A, class B>
    struct hash<std::pair<A, B>> {
        std::size_t operator()(std::pair<A, B> const &v) const noexcept {
            return boost::hash_value(v);
        }
    };

    template<class T, class A>
    struct hash<std::vector<T, A>> {
        std::size_t operator()(std::vector<T, A> const &v) const noexcept {
            return boost::hash_value(v);
        }
    };

    template<class T, class A>
    struct hash<std::list<T, A>> {
        std::size_t operator()(std::list<T, A> const &v) const noexcept {
            return boost::hash_value(v);
        }
    };

    template<class T, class A>
    struct hash<std::deque<T, A>> {
        std::size_t operator()(std::deque<T, A> const &v) const noexcept {
            return boost::hash_value(v);
        }
    };

    template<class K, class C, class A>
    struct hash<std::set<K, C, A>> {
        std::size_t operator()(std::set<K, C, A> const &v) const noexcept {
            return boost::hash_value(v);
        }
    };

    template<class K, class C, class A>
    struct hash<std::multiset<K, C, A>> {
        std::size_t operator()(std::multiset<K, C, A> const &v) const noexcept {
            return boost::hash_value(v);
        }
    };

    template<class K, class T, class C, class A>
    struct hash<std::map<K, T, C, A>> {
        std::size_t operator()(std::map<K, T, C, A> const &v) const noexcept {
            return boost::hash_value(v);
        }
    };

    template<class K, class T, class C, class A>
    struct hash<std::multimap<K, T, C, A>> {
        std::size_t operator()(std::multimap<K, T, C, A> const &v) const noexcept {
            return boost::hash_value(v);
        }
    };

}

#endif
