
#ifndef UNTITLED_IHANDLE_H
#define UNTITLED_IHANDLE_H

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
                    reinterpret_cast<std::vector<T> *>(vec)->push_back(*reinterpret_cast<T *>(elem));
                }

                virtual void resizeVector(void *vec, size_t size) {
                    reinterpret_cast<std::vector<T> *>(vec)->reserve(size);
                }

                virtual size_t hashVector(void *vec) {
                    return std::hash<std::vector<T >>()(*reinterpret_cast<std::vector<T> *>(vec));
                }

                virtual void insertSet(void *set, void *elem, size_t hash_Value) {
                    fake_hash_value = hash_Value;
                    reinterpret_cast<std::unordered_set<T, fakeHash> *>(set)->insert(*reinterpret_cast<T *>(elem));
                }

                virtual void resizeSet(void *set, size_t size) {
                    reinterpret_cast<std::unordered_set<T> *>(set)->reserve(size);
                }

                virtual size_t hashSet(void *vec) {
                    return std::hash<std::unordered_set<T >>()(*reinterpret_cast<std::unordered_set<T> *>(vec));
                }

                virtual void putMap(void *map, void *key, void *value, size_t hash_Value) {
                    fake_hash_value = hash_Value;
                    (*reinterpret_cast<std::unordered_map<T, T2, fakeHash> *>(map))[(*reinterpret_cast<T *>(key))] =
                            (*reinterpret_cast<T2 *>(value));
                }

                virtual void resizeMap(void *map, size_t size) {
                    reinterpret_cast<std::unordered_map<T, T2> *>(map)->reserve(size);
                }

                virtual size_t hashMap(void *vec) {
                    return std::hash<std::unordered_map<T, T2 >>()(*reinterpret_cast<std::unordered_map<T, T2> *>(vec));
                }

                virtual size_t hash(void *obj){
                    return std::hash<T>()(*reinterpret_cast<T*>(obj));
                }

                virtual void* newT(){
                    new T();
                }

                virtual void deleteT(void *obj){
                    delete ((T*)obj);
                }

                virtual size_t sizeT(){
                    return sizeof(T);
                }

                virtual void* newT2(){
                    new T2();
                }

                virtual void deleteT2(void *obj){
                    delete ((T2*)obj);
                }

                virtual size_t sizeT2(){
                    return sizeof(T2);
                }

            };
        }
    }
}


#endif
