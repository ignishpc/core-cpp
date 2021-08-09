
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
        void before(api::IContext &context) { set(lambda); }                                                                \
    } ignis_export(Lambda, Lambda)

namespace ignis {
    namespace executor {
        namespace core {
            namespace lambda {

                class ILambdaNoneFunction0 {
                    virtual void before(api::IContext &context) {}
                    void set(...) {}
                };

                struct ILambdaVoidFunction0 : public api::function::IVoidFunction0 {
                public:
                    void (*lambda)(api::IContext &context);
                    void set(void (*l)(api::IContext &context)) { lambda = l; }
                    void call(api::IContext &context) { lambda(context); }
                };

                template<typename Tp>
                struct ILambdaVoidFunction : public api::function::IVoidFunction<Tp> {
                    void (*lambda)(Tp &arg, api::IContext &context);
                    void set(void (*l)(Tp &arg, api::IContext &context)) { lambda = l; }
                    void call(Tp &arg, api::IContext &context) { lambda(arg, context); }
                };

                template<typename Tp1, typename Tp2>
                struct ILambdaVoidFunction2 : public api::function::IVoidFunction2<Tp> {
                    void (*lambda)(Tp1 &arg1, Tp2 &arg2, api::IContext &context);
                    void set(void (*l)(Tp1 &arg1, Tp2 &arg2, api::IContext &context)) { lambda = l; }
                    void call(Tp1 &arg1, Tp2 &arg2, api::IContext &context) { lambda(arg1, arg2, context); }
                };

                template<typename R>
                struct ILambdaFunction0 : public api::function::IFunction0<R> {
                    R (*lambda)(api::IContext &context);
                    void set(R (*l)(api::IContext &context)) { lambda = l; }
                    R call(api::IContext &context) { return lambda(context); }
                };

                template<typename Tp, typename R>
                struct ILambdaFunction : public api::function::IFunction0<Tp, R> {
                    R (*lambda)(Tp &arg, api::IContext &context);
                    void set(R (*l)(Tp &arg, api::IContext &context)) { lambda = l; }
                    R call(Tp &arg, api::IContext &context) { return lambda(arg, context); }
                };

                template<typename Tp1, typename Tp2, typename R>
                struct ILambdaFunction2 : public api::function::IFunction2<Tp, R> {
                    R (*lambda)(Tp1 &arg1, Tp2 &arg2, api::IContext &context);
                    void set(R (*l)(Tp1 &arg1, Tp2 &arg2, api::IContext &context)) { lambda = l; }
                    R call(Tp1 &arg1, Tp2 &arg2, api::IContext &context) { return lambda(arg1, arg2, context); }
                };

                ILambdaNoneFunction0 *call(...) { return nullptr; }

                template<typename T>
                ILambdaVoidFunction0<T> *call(void (T::*a)(api::IContext &context) const) {
                    return nullptr;
                }

                template<typename T, typename P>
                ILambdaVoidFunction<P> *call(void (T::*a)(P, api::IContext &context) const) {
                    return nullptr;
                }

                template<typename T, typename P1, typename P2>
                ILambdaVoidFunction2<P1, P2> *call(void (T::*a)(P1, P2, api::IContext &context) const) {
                    return nullptr;
                }

                template<typename T, typename R>
                ILambdaFunction0<R> *call(R (T::*a)(api::IContext &context) const) {
                    return nullptr;
                }

                template<typename T, typename R, typename P>
                ILambdaFunction<P, R> *call(R (T::*a)(P, api::IContext &context) const) {
                    return nullptr;
                }

                template<typename T, typename R, typename P1, typename P2>
                ILambdaFunction2<P1, P2, R> *call(R (T::*a)(P1, P2, api::IContext &context) const) {
                    return nullptr;
                }

            }// namespace lambda
        }    // namespace core
    }        // namespace executor
}// namespace ignis

#endif//IGNIS_ILAMBDA_H
