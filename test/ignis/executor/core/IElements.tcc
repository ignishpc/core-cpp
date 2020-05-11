
#include "IElements.h"
#include <utility>

template<typename Key, typename Value>
struct ignis::executor::core::IElements<std::pair<Key, Value>> {
    static api::IVector <std::pair<Key, Value>> create(int n, int seed) {
        auto keys = IElements<Key>::create(n, seed);
        auto values = IElements<Value>::create(n, seed);
        api::IVector<std::pair<Key, Value>> v;
        for (int i = 0; i < n; i++) {
            v.emplace_back(keys[i], values[i]);
        }
        return v;
    }
};

template<>
struct ignis::executor::core::IElements<bool> {
    static api::IVector<bool> create(int n, int seed) {
        std::srand(seed);
        api::IVector<bool> v;
        for (int i = 0; i < n; i++) {
            v.push_back((std::rand() % 2) == 1);
        }
        return v;
    }
};

template<>
struct ignis::executor::core::IElements<int> {
    static api::IVector<int> create(int n, int seed) {
        std::srand(seed);
        api::IVector<int> v;
        for (int i = 0; i < n; i++) {
            v.push_back(std::rand() % n);
        }
        return v;
    }
};

template<>
struct ignis::executor::core::IElements<std::string> {
    static api::IVector <std::string> create(int n, int seed) {
        std::srand(seed);
        api::IVector<std::string> v;
        for (int i = 0; i < n; i++) {
            v.push_back(std::to_string(std::rand() % n));
        }
        return v;
    }
};

template<>
struct ignis::executor::core::IElements<uint8_t> {
    static api::IVector <uint8_t> create(int n, int seed) {
        std::srand(seed);
        api::IVector<uint8_t> v;
        for (int i = 0; i < n; i++) {
            v.push_back((uint8_t) (std::rand() % 256));
        }
        return v;
    }
};

template<>
struct ignis::executor::core::IElements<std::pair<int, std::string>> {
    static api::IVector <std::pair<int, std::string>> create(int n, int seed) {
        std::srand(seed);
        api::IVector<std::pair<int, std::string>> v;
        for (int i = 0; i < n; i++) {
            v.emplace_back(std::rand() % n, std::to_string(std::rand() % n));
        }
        return v;
    }
};