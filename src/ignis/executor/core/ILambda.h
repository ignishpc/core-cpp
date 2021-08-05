
#ifndef IGNIS_ILAMBDA_H
#define IGNIS_ILAMBDA_H

#include "ignis/executor/api/function/IFunction.h"
#include "ignis/executor/api/function/IFunction0.h"
#include "ignis/executor/api/function/IFunction2.h"
#include "ignis/executor/api/function/IVoidFunction.h"
#include "ignis/executor/api/function/IVoidFunction0.h"
#include "ignis/executor/api/function/IVoidFunction2.h"
#include <type_traits>


#define ignis_lambda_export(lambda)                                                                                    \
    class Lambda : public typename std::remove_reference<decltype(*Test::call(&decltype(lambda)::operator()))>::type { \
    public:                                                                                                            \
        void before(IContext &context) { set(lambda); }                                                                \
    }                                                                                                                  \
    ignis_export(Lambda, Lambda)

namespace ignis {
    namespace executor {
        namespace core {
            namespace lambda {

                class ILambdaNoneFunction0 {
                    virtual void before(IContext &context) {}
                    void set(void (*l)()) {}
                };

                struct ILambdaVoidFunction0 : public api::function::IVoidFunction0 {
                public:
                    void (*lambda)();
                    void set(void (*l)()) { lambda = l; }
                    void call() { lambda(); }
                };

                template<typename Tp>
                struct ILambdaVoidFunction : public api::function::IVoidFunction<Tp> {
                    void (*lambda)(Tp &arg);
                    void set(void (*l)(Tp &arg)) { lambda = l; }
                    void call(Tp &arg) { lambda(arg); }
                };

                template<typename Tp1, typename Tp2>
                struct ILambdaVoidFunction2 : public api::function::IVoidFunction2<Tp> {
                    void (*lambda)(Tp1 &arg1, Tp2 &arg2);
                    void set(void (*l)(Tp1 &arg1, Tp2 &arg2)) { lambda = l; }
                    void call(Tp1 &arg1, Tp2 &arg2) { lambda(arg1, arg2); }
                };

                template<typename R>
                struct ILambdaFunction0 : public api::function::IFunction0<R> {
                    R (*lambda)();
                    void set(R (*l)()) { lambda = l; }
                    R call() { return lambda(); }
                };

                template<typename Tp, typename R>
                struct ILambdaFunction : public api::function::IFunction0<Tp, R> {
                    R (*lambda)(Tp &arg);
                    void set(R (*l)(Tp &arg)) { lambda = l; }
                    R call(Tp &arg) { return lambda(arg); }
                };

                template<typename Tp1, typename Tp2, typename R>
                struct ILambdaFunction2 : public api::function::IFunction2<Tp, R> {
                    R (*lambda)(Tp1 &arg1, Tp2 &arg2);
                    void set(R (*l)(Tp1 &arg1, Tp2 &arg2)) { lambda = l; }
                    R call(Tp1 &arg1, Tp2 &arg2) { return lambda(arg1, arg2); }
                };

                ILambdaNoneFunction0 *call(...) { return nullptr; }

                template<typename T>
                ILambdaVoidFunction0<T> *call(void (T::*a)() const) {
                    return nullptr;
                }

                template<typename T, typename P>
                ILambdaVoidFunction<P> *call(void (T::*a)(P) const) {
                    return nullptr;
                }

                template<typename T, typename P1, typename P2>
                ILambdaVoidFunction2<P1, P2> *call(void (T::*a)(P1, P2) const) {
                    return nullptr;
                }

                template<typename T, typename R>
                ILambdaFunction0<R> *call(R (T::*a)() const) {
                    return nullptr;
                }

                template<typename T, typename R, typename P>
                ILambdaFunction<P, R> *call(R (T::*a)(P) const) {
                    return nullptr;
                }

                template<typename T, typename R, typename P1, typename P2>
                ILambdaFunction2<P1, P2, R> *call(R (T::*a)(P1, P2) const) {
                    return nullptr;
                }

            }// namespace lambda
        }// namespace core
    }    // namespace executor
}// namespace ignis

#endif//IGNIS_ILAMBDA_H
