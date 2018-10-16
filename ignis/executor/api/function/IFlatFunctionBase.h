
#ifndef IGNIS_IFLATFUNCTIONBASE_H
#define IGNIS_IFLATFUNCTIONBASE_H

#include "../IRegister.h"
#include "../IManager.h"
#include "../IContext.h"
#include "../IWriteIterator.h"
#include "../Iterable.h"

namespace ignis {
    namespace executor {
        namespace api {
            namespace function {

                template<typename T, typename R>
                class IFlatFunctionBase {
                private:
                    Type_ptr<IManager<T>> var_type_t;
                    Type_ptr<IManager<R>> var_type_r;
                public:
                    decltype(var_type_t) type_t() { return var_type_t; }

                    decltype(var_type_r) type_r() { return var_type_r; }

                    virtual Iterable<R> call(T &t, IContext &context) = 0;

                    virtual void write(T &t, IContext &context, IWriteIterator<R> &writer) {
                        auto it = call(t, context).readIterator();
                        while (it->hasNext()) {
                            writer.write((R&&)it->next());
                        }
                    }

                };
            }
        }
    }
}

#endif
