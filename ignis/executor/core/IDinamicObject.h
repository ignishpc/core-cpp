
#ifndef INGIS_DINAMICOBJECT_H
#define IGNIS_DINAMICOBJECT_H

#include <string>
#include "../../exceptions/IInvalidArgument.h"
#include <memory>

namespace ignis {
    namespace executor {
        namespace core {

            class ObjectLoader {
            public:
                ObjectLoader(const std::string &name);

                ObjectLoader(ObjectLoader &other) = delete;

                ObjectLoader(ObjectLoader &&other) = delete;

                void *getObject();

                virtual ~ObjectLoader();

            private:

                void *library;
                void *object;

                void (*destructor)(void *object);

            };

            template<typename T>
            class IDinamicObject {
            public:

                IDinamicObject(const std::string &name) {
                    loader = std::make_shared<ObjectLoader>(name);
                }

                IDinamicObject(const std::string &name, bool bytes) {
                    if(bytes){
                        throw exceptions::IInvalidArgument("C++ not support function serialization");
                    }
                    loader = std::make_shared<ObjectLoader>(name);
                }

                T *operator->() const {
                    return loader->getObject();
                }

                T *get() const {
                    return loader->getObject();
                }

            private:
                std::shared_ptr<ObjectLoader> loader;
            };

        }
    }
}
#endif
