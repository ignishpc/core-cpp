
#ifndef IGNIS_ICONTAINER_H
#define IGNIS_ICONTAINER_H

#include <memory>
#include "IHandle.h"

namespace ignis {
    namespace data {
        namespace serialization {
            typedef char Any;

            template<typename T=bool, template<typename, typename> typename IH=IHandle>
            class ISimpleContainer {
            public:
                virtual std::shared_ptr<IHandle<>> newInstance() {
                    return std::shared_ptr<IHandle<>>((IHandle<> *) new IH<T, bool>());
                }

                template<typename T2>
                std::shared_ptr<IHandle<>> getInstance(T2 *n) {
                    return std::shared_ptr<IHandle<>>((IHandle<> *) new IH<T2, T>());
                }

                virtual std::shared_ptr<IHandle<>> instance(bool *t) {
                    return getInstance<>(t);
                }

                virtual std::shared_ptr<IHandle<>> instance(__int8_t *t) {
                    return getInstance<>(t);
                }

                virtual std::shared_ptr<IHandle<>> instance(__int16_t *t) {
                    return getInstance<>(t);
                }

                virtual std::shared_ptr<IHandle<>> instance(__int32_t *t) {
                    return getInstance<>(t);
                }

                virtual std::shared_ptr<IHandle<>> instance(__int64_t *t) {
                    return getInstance<>(t);
                }

                virtual std::shared_ptr<IHandle<>> instance(double *t) {
                    return getInstance<>(t);
                }

                virtual std::shared_ptr<IHandle<>> instance(std::string *t) {
                    return getInstance<>(t);
                }

                virtual std::shared_ptr<IHandle<>> instance(std::vector<Any> *t) {
                    return getInstance<>(t);
                }

                virtual std::shared_ptr<IHandle<>> instance(std::unordered_set<Any> *t) {
                    return getInstance<>(t);
                }

                virtual std::shared_ptr<IHandle<>> instance(std::unordered_map<Any, Any> *t) {
                    return getInstance<>(t);
                }

                virtual std::shared_ptr<IHandle<>> instance(std::pair<Any, Any> *t) {
                    return getInstance<>(t);
                }
            };

            template<typename T=bool, template<typename, typename> typename IH=IHandle>
            class IContainer : public ISimpleContainer<T, IH> {
            public:

                virtual std::shared_ptr<IHandle<>> newInstance(IContainer<> *sc = NULL) {
                    if (sc != NULL) {
                        return sc->instance((T *) NULL);
                    }
                    return ISimpleContainer<T, IH>::newInstance();
                }

            };
        }
    }
}

#endif
