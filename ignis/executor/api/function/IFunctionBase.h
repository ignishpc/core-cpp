
#ifndef IGNIS_IFUNCTIONBASE_H
#define IGNIS_IFUNCTIONBASE_H

#include "../IRegister.h"
#include "../IContext.h"
#include "../IManager.h"
#include "../IWriteIterator.h"

namespace ignis {
    namespace executor {
        namespace api {
            namespace function {

                template<typename T, typename R>
                class IFunctionBase {
                private:
                    Type_ptr<IManager < T>>var_type_t;
                    Type_ptr<IManager < R>>var_type_r;
                    Type_ptr<IPairManager < R, T>>var_pair_rt;

                public:
                    decltype(var_type_t) type_t() { return var_type_t; }

                    decltype(var_type_r) type_r() { return var_type_r; }

                    decltype(var_pair_rt) pair_rt() { return var_pair_rt; }

                    virtual R call(T &t, IContext &context) = 0;

                    virtual void write(T &t, IContext &context, IWriteIterator <R> &writer) {
                        writer.write(call(t, context));
                    }

                    virtual void
                    write(T &t, IContext &context, IWriteIterator<typename IPairManager<R, T>::Class> &writer) {
                        writer.write(typename IPairManager<R, T>::Class(call(t, context), t));
                    }

                };
            }
        }
    }
}

#endif
