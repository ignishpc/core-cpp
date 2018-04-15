
#ifndef IGNIS_IDELETER_H
#define IGNIS_IDELETER_H

namespace ignis {
    namespace data {
        namespace serialization {

            template<typename T>
            struct IDeleterType {
                void operator()(T *obj) {
                    delete obj;
                }
            };

            template<typename T>
            class IDeleter {
            public:
                virtual void operator()(T *obj) {
                    deleter(obj);
                }

            private:
                IDeleterType<T> deleter;
            };
        }
    }
}

#endif
