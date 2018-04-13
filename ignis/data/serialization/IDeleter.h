
#ifndef IGNIS_IDELETER_H
#define IGNIS_IDELETER_H

namespace ignis {
    namespace data {
        namespace serialization {

            template<typename T>
            struct IDeleterType {
                void deleteObject(T *obj) {
                    delete obj;
                }
            };

            template<typename T>
            class IDeleter {
            public:
                virtual void deleteObject(T *obj) {
                    deleter.deleteObject(obj);
                }

            private:
                IDeleterType<T> deleter;
            };
        }
    }
}

#endif
