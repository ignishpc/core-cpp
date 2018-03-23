
#ifndef UNTITLED_ITYPE_H
#define UNTITLED_ITYPE_H

#include <memory>
#include "IHandle.h"
#

namespace ignis {
    namespace data {
        namespace serialization {

            typedef char Any;

            template<typename T=bool, template<typename, typename> typename C=IHandle>
            class IExtendedType {

                template<typename T2>
                std::shared_ptr<IHandle<>> new_instance(T2 *n) {
                    return std::shared_ptr<IHandle<>>((IHandle<> *) new C<T, T2>());
                }

            public:
                virtual std::shared_ptr<IHandle<>> instance(bool *t) {
                    return new_instance<>(t);
                }

                virtual std::shared_ptr<IHandle<>> instance(__int8_t *t) {
                    return new_instance<>(t);
                }

                virtual std::shared_ptr<IHandle<>> instance(__int16_t *t) {
                    return new_instance<>(t);
                }

                virtual std::shared_ptr<IHandle<>> instance(__int32_t *t) {
                    return new_instance<>(t);
                }

                virtual std::shared_ptr<IHandle<>> instance(__int64_t *t) {
                    return new_instance<>(t);
                }

                virtual std::shared_ptr<IHandle<>> instance(double *t) {
                    return new_instance<>(t);
                }

                virtual std::shared_ptr<IHandle<>> instance(std::string *t) {
                    return new_instance<>(t);
                }

                virtual std::shared_ptr<IHandle<>> instance(std::vector<Any> *t) {
                    return new_instance<>(t);
                }

                virtual std::shared_ptr<IHandle<>> instance(std::unordered_set<Any> *t) {
                    return new_instance<>(t);
                }

                virtual std::shared_ptr<IHandle<>> instance(std::unordered_map<Any, Any> *t) {
                    return new_instance<>(t);
                }

                virtual std::shared_ptr<IHandle<>> instance(std::pair<Any, Any> *t) {
                    return new_instance<>(t);
                }

            };
        }
    }
}
#endif
