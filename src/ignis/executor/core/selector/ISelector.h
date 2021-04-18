
#ifndef IGNIS_ISELECTOR_H
#define IGNIS_ISELECTOR_H


#include "IGeneralActionSelector.h"
#include "IGeneralSelector.h"
#include "IKeySelector.h"
#include "ITypeSelector.h"
#include "IValueSelector.h"

#define ignis_export(name, class)                                                                                      \
    extern "C" ignis::executor::core::selector::ISelectorGroup *name##_constructor() {                                 \
        return new ignis::executor::core::selector::ISelectorGroupImpl<class>(#name);                                       \
    }                                                                                                                  \
                                                                                                                       \
    extern "C" void name##_destructor(ignis::executor::core::selector::ISelectorGroupImpl<class> *obj) { delete obj; }

#define ignis_export_with_key(name, class, keyclass)                                                                   \
    extern "C" ignis::executor::core::selector::ISelectorGroup *name##_constructor() {                                 \
        return new ignis::executor::core::selector::ISelectorGroupKeyImpl<keyclass, class>(#name);                          \
    }                                                                                                                  \
                                                                                                                       \
    extern "C" void name##_destructor(ignis::executor::core::selector::ISelectorGroupKeyImpl<keyclass, class> *obj) {  \
        delete obj;                                                                                                    \
    }

#define ignis_export_with_value(name, class, valueclass)                                                               \
    extern "C" ignis::executor::core::selector::ISelectorGroup *name##_constructor() {                                 \
        return new ignis::executor::core::selector::ISelectorGroupValueImpl<valueclass, class>(#name);                      \
    }                                                                                                                  \
                                                                                                                       \
    extern "C" void name##_destructor(                                                                                 \
            ignis::executor::core::selector::ISelectorGroupValueImpl<valueclass, class> *obj) {                        \
        delete obj;                                                                                                    \
    }

#define create_ignis_library(f, ...) const char *__ignis_library__[] = {#f, #__VA_ARGS__, NULL}

namespace ignis {
    namespace executor {
        namespace core {
            namespace selector {

                class ISelectorGroup {
                public:
                    ISelectorGroup(const std::string &name) : name(name) {}

                    virtual RTTInfo info() = 0;

                    virtual void loadClass(api::IContext &context) = 0;

                    const std::string name;
                    std::vector<std::shared_ptr<ITypeSelector>> args;
                    std::shared_ptr<IGeneralSelector> general;
                    std::shared_ptr<IGeneralActionSelector> general_action;
                    std::shared_ptr<IKeySelector> key;
                    std::shared_ptr<IValueSelector> value;
                };

                template<typename Tp>
                class ISelectorGroupImpl : public ISelectorGroup {
                public:
                    virtual RTTInfo info() { return RTTInfo::from<Tp>(); }

                    virtual void loadClass(api::IContext &context) { general->loadClass(context); }

                    ISelectorGroupImpl(const std::string &name) : ISelectorGroup(name) {
                        args = ITypeSelectorExtractor().extract<Tp>();
                        general = std::make_shared<IGeneralSelectorImpl<Tp>>();
                        general_action = std::make_shared<IGeneralActionSelectorImpl<Tp>>();
                        key = std::make_shared<IKeySelector>();
                        value = std::make_shared<IValueSelector>();
                    }
                };

                template<typename K, typename Tp>
                class ISelectorGroupKeyImpl : public ISelectorGroupImpl<Tp> {
                public:
                    ISelectorGroupKeyImpl(const std::string &name) : ISelectorGroupImpl<Tp>(name) {
                        this->key = std::make_shared<IKeySelectorImpl<K, Tp>>();
                    }
                };

                template<typename K, typename Tp>
                class ISelectorGroupValueImpl : public ISelectorGroupImpl<Tp> {
                public:
                    ISelectorGroupValueImpl(const std::string &name) : ISelectorGroupImpl<Tp>(name) {
                        this->value = std::make_shared<IValueSelectorImpl<K, Tp>>();
                    }
                };


            }// namespace selector
        }    // namespace core
    }        // namespace executor
}// namespace ignis


#endif
