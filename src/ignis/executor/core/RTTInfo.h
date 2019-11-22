
#ifndef IGNIS_RTTINFO_H
#define IGNIS_RTTINFO_H

#include <string>
#include <typeinfo>

namespace ignis {
    namespace executor {
        namespace core {
            class RTTInfo {
            private:
                const std::type_info &info;
            public:
                /*
                 *
                 */
                RTTInfo(const std::type_info &info);

                template<typename Tp>
                static RTTInfo from() {
                    return RTTInfo(typeid(Tp));
                }

                const std::type_info &getInfo() const;

                /*
                 * Get a name with c++ standard. Ex: std::string or int*
                 */
                std::string getStandardName() const;

                /*
                 * Type is primitive. Ex: int, float...
                 */
                bool isPrimitive() const;

                /*
                 * Type is an array.
                 */
                bool isArray() const;

                /*
                 * Type is a function.
                 */
                bool isFunction() const;

                /*
                 * Type is an enum
                 */
                bool isEnum() const;

                /*
                 * Type is a class
                 */
                bool isClass() const;

                /*
                 * Type is a class and only have a single public Base
                 */
                bool isSimpleClass() const;

                /*
                 * Type is a class and have more than one father or a single non public Base
                 */
                bool isVirtalClass() const;

                /*
                 * Type is a base class of the argument class
                 */
                bool isBaseOf(const std::type_info &info) const;

                /*
                 * Type is a derived class of the argument class
                 */
                bool isDerivedOf(const std::type_info &info) const;

                /*
                 * Get the number of base class
                 */
                unsigned int getNumOfBases() const;

                /*
                 * Get type of one of base classes
                 * @param p position of base
                 */
                const std::type_info &getBase(int p) const;

                /*
                 * Get base class or the first if there are more than one
                 */
                const std::type_info &getBase() const;

                /*
                 * Type is void
                 */
                bool isVoid() const;

                /*
                 * Type is a pointer
                 */
                bool isPointer() const;

                /*
                * Type of pointed to class
                */
                const std::type_info &getClassPointed() const;

                /*
                 * Type is a pointer to a class member
                 */
                bool isMemberPointer() const;

                /*
                 * Type of class of pointed to member
                 */
                const std::type_info &getClassMemberPointed() const;

                friend bool operator==(const RTTInfo &lhs, const RTTInfo &rhs);

                friend bool operator!=(const RTTInfo &lhs, const RTTInfo &rhs);
            };

            bool operator==(const RTTInfo &lhs, const RTTInfo &rhs);

            bool operator!=(const RTTInfo &lhs, const RTTInfo &rhs);

        }
    }
}

#endif
