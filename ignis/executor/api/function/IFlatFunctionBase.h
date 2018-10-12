
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
                public:
                    const IManager<T> type_t;
                    const IManager<R> type_r;

                    virtual Iterable<R> call(T &t, IContext &context) = 0;

                    virtual void write(T &t, IContext &context, IWriteIterator<R> &writer) {
                        auto it = call(t, context).readIterator();
                        while(it->hasNext()){
                            writer.write(it->next());
                        }
                    }

                };
            }
        }
    }
}

#endif
