

#ifndef IGNIS_ISELECTORUTILS_H
#define IGNIS_ISELECTORUTILS_H

#include <utility>

namespace ignis {
    namespace executor {
        namespace core {
            namespace selector {

                template<typename, typename>
                struct ISame {
                    typedef void result;
                };

                template<typename Tp>
                struct ISame<Tp, Tp> {
                    typedef Tp result;
                };

                template<typename Tp>
                class IHasLess {

                    template<typename Tp2>
                    struct Check {

                        Tp operator()() {}
                    };

                    template<typename Key, typename Value>
                    struct Check<std::pair<Key, Value>> {

                        typename ISame<typename IHasLess<Key>::result, typename IHasLess<Value>::result>::result
                        operator()() {}

                    };

                    template<typename C>
                    static const C *has(decltype(Check<C>()() < Check<C>()()) *flag) { return nullptr; }

                    template<typename C>
                    static void has(...) {}

                public:
                    typedef decltype(has<Tp>(nullptr)) result;
                };


                template<typename Tp>
                class IHasHash {

                    template<typename Tp2>
                    struct Check {

                        Tp operator()() {}
                    };

                    template<typename Key, typename Value>
                    struct Check<std::pair<Key, Value>> {

                        typename ISame<typename IHasLess<Key>::result, typename IHasLess<Value>::result>::result
                        operator()() {}

                    };

                    template<typename C>
                    static const C *has(decltype(&std::hash<C>::operator()) *flag) { return nullptr; }

                    template<typename C>
                    static void has(...) {}

                public:
                    typedef decltype(has<Tp>(nullptr)) result;
                };

            }
        }
    }
}


#endif
