
#ifndef IGNIS_IHANDLE_H
#define IGNIS_IHANDLE_H

#include <memory>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "IHash.h"

namespace ignis {
    namespace data {
        namespace serialization {
            static thread_local size_t fake_hash_value;

            template<typename T=bool, typename T2=T>
            class IHandle {
            public:

                struct fakeHash {

                    size_t operator()(T const &v) const noexcept {
                        return fake_hash_value;
                    }
                };

                virtual inline void pushVector(void *vec, void *elem) {
                    reinterpret_cast<std::vector<T> *>(vec)->push_back(std::move(*reinterpret_cast<T *>(elem)));
                }

                virtual inline void resizeVector(void *vec, size_t size) {
                    reinterpret_cast<std::vector<T> *>(vec)->reserve(size);
                }

                virtual inline void insertSet(void *set, void *elem, size_t hash_Value) {
                    fake_hash_value = hash_Value;
                    reinterpret_cast<std::unordered_set<T, fakeHash> *>(set)->insert(std::move(*reinterpret_cast<T *>(elem)));
                }

                virtual inline void resizeSet(void *set, size_t size) {
                    reinterpret_cast<std::unordered_set<T> *>(set)->reserve(size);
                }

                virtual inline void putMap(void *map, void *key, void *value, size_t hash_Value) {
                    fake_hash_value = hash_Value;
                    (*reinterpret_cast<std::unordered_map<T, T2, fakeHash> *>(map))[(*reinterpret_cast<T *>(key))] =
                            std::move((*reinterpret_cast<T2 *>(value)));
                }

                virtual inline void resizeMap(void *map, size_t size) {
                    reinterpret_cast<std::unordered_map<T, T2> *>(map)->reserve(size);
                }

                virtual inline void* firstPair(void *pair) {
                    return &(*reinterpret_cast<std::pair<T, T2> *>(pair)).first;
                }

                virtual inline void* secondPair(void *pair) {
                    return &(*reinterpret_cast<std::pair<T, T2> *>(pair)).second;
                }

                virtual inline void move(T& source, T& target){
                    target = std::move(source);
                }

                virtual inline void move2(T& source, T& target){
                    target = std::move(source);
                }

                virtual inline void* newT(){
                    return new T();
                }

                virtual inline void deleteT(void *obj){
                    delete ((T*)obj);
                }

                virtual inline size_t sizeT(){
                    return sizeof(T);
                }

                virtual inline void* newT2(){
                    return new T2();
                }

                virtual inline void deleteT2(void *obj){
                    delete ((T2*)obj);
                }

                virtual inline size_t sizeT2(){
                    return sizeof(T2);
                }

            };
        }
    }
}


#endif
