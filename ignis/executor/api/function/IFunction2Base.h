
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
                public:
                    const IManager <T1> type_t1;
                    const IManager <T2> type_t2;
                    const IManager <R> type_r;
                    typedef char Any;

                    virtual R call(T1 &t1, T2 &t2, IContext &context) = 0;

                    virtual void write(T1 &t1, T2 &t2, IContext &context, IWriteIterator <R> &writer) {
                        writer.write(call(t1, t2, context));
                    }

                    virtual void writeReduceByKey(typename IPairManager<Any, T1>::Class &pt1,
                                                  typename IPairManager<Any, T2>::Class &pt2,//It is a copy
                                                  IContext &context,
                                                  IWriteIterator<typename IPairManager<Any, R>::Class> &writer,
                                                  IPairManager <Any, T2> &mp) {
                        T1 &t1 = mp.second((typename IPairManager<Any, T2>::Class &) pt1);
                        T2 &t2 = mp.second(pt2);
                        t2 = (T1 &) call(t1, t2, context);
                        writer.write(pt2);
                    }

                };
            }
        }
    }
}

#endif
