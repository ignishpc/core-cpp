
#ifndef INGIS_DINAMICOBJECT_H
#define INGIS_DINAMICOBJECT_H

#include "../../IHeaders.h"

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

                T *operator->() const {
                    return (T *) loader->getObject();
                }

                T &operator*() const {
                    return *((T *)loader->getObject());
                }

                T *get() const {
                    return (T *) loader->getObject();
                }

            private:
                std::shared_ptr<ObjectLoader> loader;
            };

        }
    }
}
#endif
