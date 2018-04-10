
#ifndef IGNIS_IDELETER_H
#define IGNIS_IDELETER_H

namespace ignis {
    namespace data {
        namespace serialization {

            template <typename T>
            class IDeleter {
            public:
                virtual void deleteObject(T* obj){
                    delete obj;
                }
            };
        }
    }
}

#endif
