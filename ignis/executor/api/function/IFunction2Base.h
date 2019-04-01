
#ifndef IGNIS_IFUNCTION2BASE_H
#define IGNIS_IFUNCTION2BASE_H

#include "../IRegister.h"
#include "../IManager.h"
#include "../IContext.h"
#include "../IWriteIterator.h"

namespace ignis {
    namespace executor {
        namespace api {
            namespace function {

                template<typename T1, typename T2, typename R>
                class IFunction2Base {
                private:
                    Type_ptr<IManager < T1>> var_type_t1;
                    Type_ptr<IManager < T2>> var_type_t2;
                    Type_ptr<IManager < R>> var_type_r;
                public:
                    decltype(var_type_t1) type_t1() { return var_type_t1; }

                    decltype(var_type_t2) type_t2() { return var_type_t2; }

                    decltype(var_type_r) type_r() { return var_type_r; }

                    virtual R call(T1 &t1, T2 &t2, IContext &context) = 0;

                    virtual void write(T1 &t1, T2 &t2, IContext &context, IWriteIterator <R> &writer) {
                        writer.write(call(t1, t2, context));
                    }

                };
            }
        }
    }
}

#endif
