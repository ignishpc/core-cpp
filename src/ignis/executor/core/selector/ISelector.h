
#ifndef IGNIS_ISELECTOR_H
#define IGNIS_ISELECTOR_H


#include "ITypeSelector.h"
#include "IGeneralSelector.h"
#include "IGeneralActionSelector.h"

#define ignis_export(name, class)\
extern "C" ignis::executor::core::selector::ISelectorGroup* name##_constructor(){\
    return new ignis::executor::core::selector::ISelectorGroupImpl<class>();\
}\
\
extern "C" void name##_destructor(ignis::executor::core::selector::ISelectorGroupImpl<class>* obj){\
    delete obj;\
}

namespace ignis {
    namespace executor {
        namespace core {
            namespace selector {

                class ISelectorGroup {
                public:
                    virtual RTTInfo info() = 0;

                    typename ITypeSelectorExtractor::Args args;
                    std::shared_ptr<IGeneralSelector> general;
                    std::shared_ptr<IGeneralActionSelector> general_action;
                };

                template<typename Tp>
                class ISelectorGroupImpl : public ISelectorGroup {
                public:
                    virtual RTTInfo info() { return RTTInfo::from<Tp>(); }

                    ISelectorGroupImpl() {
                        args = ITypeSelectorExtractor().extract<Tp>();
                        general = std::make_shared<IGeneralSelectorImpl<Tp>>();
                        general_action = std::make_shared<IGeneralActionSelectorImpl<Tp>>();
                    }
                };

            }
        }
    }
}


#endif
