
#ifndef IGNIS_IFUNCTIONBASE_H
#define IGNIS_IFUNCTIONBASE_H

#include "../IRegister.h"
#include "../IManager.h"
#include "../IContext.h"
#include "../IWriteIterator.h"

namespace ignis {
    namespace executor {
        namespace api {
            namespace function {

                template<typename T, typename R>
                class IFunctionBase {
                public:
                    const IManager <T> type_t;
                    const IManager <R> type_r;
                    const IPairManager <R, T> key_t;
                    const IPairManager<R, typename ICollectionManager<T>::Class> group_t;

                    virtual R call(T &t, IContext &context) = 0;

                    virtual void write(T &t, IContext &context, IWriteIterator <R> &writer) {
                        writer.write(call(t, context));
                    }

                    virtual void
                    writeWithKey(T &t, IContext &context, IWriteIterator<typename IPairManager<R, T>::Class> &writer) {
                        writer.write(typename IPairManager<R, T>::Class(call(t, context), t));
                    }

                };
            }
        }
    }
}

#endif
