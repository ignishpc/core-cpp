
#ifndef IGNIS_IFUNCTIONIMPORT_H
#define IGNIS_IFUNCTIONIMPORT_H

#include "executor/core/selector/ISelector.h"
#include "executor/api/IContext.h"

namespace ignis {
    namespace executor {
        namespace api {
            namespace function {
                template<typename T>
                class IFunctionImport {
                public:
                    typedef IFunctionImport<T> _IFunctionImport_type;
                    typedef T _T_type;

                    virtual void before(IContext &context) {}

                    virtual void call(IContext &context) {}

                    virtual void after(IContext &context) {}

                };
            }
        }
    }
}

#endif

