
#ifndef IGNIS_ISELECTOR_H
#define IGNIS_ISELECTOR_H


#include "ITypeSelector.h"
#include "IGeneralSelector.h"
#include "IGeneralActionSelector.h"
#include "IKeySelector.h"
#include "IValueSelector.h"

#define ignis_export(name, class)\
extern "C" ignis::executor::core::selector::ISelectorGroup* name##_constructor(){\
    return new ignis::executor::core::selector::ISelectorGroupImpl<class>();\
}\
\
extern "C" void name##_destructor(ignis::executor::core::selector::ISelectorGroupImpl<class>* obj){\
    delete obj;\
}

#define ignis_export_with_key(name, class, keyclass)\
extern "C" ignis::executor::core::selector::ISelectorGroup* name##_constructor(){\
    return new ignis::executor::core::selector::ISelectorGroupKeyImpl<keyclass, class>();\
}\
\
extern "C" void name##_destructor(ignis::executor::core::selector::ISelectorGroupKeyImpl<keyclass, class>* obj){\
    delete obj;\
}

#define ignis_export_with_value(name, class, valueclass)\
extern "C" ignis::executor::core::selector::ISelectorGroup* name##_constructor(){\
    return new ignis::executor::core::selector::ISelectorGroupValueImpl<valueclass, class>();\
}\
\
extern "C" void name##_destructor(ignis::executor::core::selector::ISelectorGroupValueImpl<valueclass, class>* obj){\
    delete obj;\
}

namespace ignis {
    namespace executor {
        namespace core {
            namespace selector {

                class ISelectorGroup {
                public:
                    virtual RTTInfo info() = 0;

                    virtual void loadClass(api::IContext &context) = 0;

                    typename ITypeSelectorExtractor::Args args;
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

                    ISelectorGroupImpl() {
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
                    ISelectorGroupKeyImpl() : ISelectorGroupImpl<Tp>() {
                        this->key = std::make_shared<IKeySelectorImpl<K, Tp>>();
                    }
                };

                template<typename K, typename Tp>
                class ISelectorGroupValueImpl : public ISelectorGroupImpl<Tp> {
                public:
                    ISelectorGroupValueImpl() : ISelectorGroupImpl<Tp>() {
                        this->value = std::make_shared<IValueSelectorImpl<K, Tp>>();
                    }
                };


            }
        }
    }
}


#endif

