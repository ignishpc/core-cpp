
#ifndef IGNIS_IVARIABLE_H
#define IGNIS_IVARIABLE_H

#include "RTTInfo.h"
#include "exception/ILogicError.h"
#include "protocol/IObjectProtocol.h"
#include <memory>
#include <string>
#include <utility>

namespace ignis {
    namespace executor {
        namespace core {


            template<typename Tp>
            class IBasicVariable;

            class IBytesVariable;

            class IVariable {
            public:
                virtual bool decoded() = 0;

                template<typename Tp>
                IBasicVariable<Tp> &basic() {
                    return reinterpret_cast<IBasicVariable<Tp> &>(*this);
                }

                IBytesVariable &bytes();
            };

            template<typename Tp>
            class IBasicVariable : public IVariable {
            public:
                IBasicVariable() {}

                IBasicVariable(const Tp &value) : value(value) {}

                IBasicVariable(Tp &&value) : value(value) {}

                bool decoded() { return true; }

                Tp &get() { return get(RTTInfo::from<Tp>()); }

            private:
                virtual Tp &get(const RTTInfo &tp) {
                    if (tp != RTTInfo::from<Tp>()) {
                        throw exception::ILogicError("Error: " + tp.getStandardName() + " is not " +
                                                     RTTInfo::from<Tp>().getStandardName());
                    }
                    return value;
                }

                Tp value;
            };

            class IBytesVariable : public IVariable {
            public:
                IBytesVariable(const std::string &value);

                IBytesVariable(std::string &&value);

                bool decoded();

                template<typename Tp>
                std::shared_ptr<IVariable> decode() {
                    return std::make_shared<IBasicVariable<Tp>>(protocol()->readObject<Tp>());
                }

            private:
                std::shared_ptr<core::protocol::IObjectProtocol> protocol();

                std::string value;
            };


        }// namespace core
    }    // namespace executor
}// namespace ignis

#endif
