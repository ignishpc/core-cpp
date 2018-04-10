
#ifndef IGNIS_RTTINFO_H
#define IGNIS_RTTINFO_H

#include <string>
#include <typeinfo>

namespace ignis {
    namespace data {
        class RTTInfo {
        private:
            const std::type_info &info;
        public:
            /*
             *
             */
            RTTInfo(const std::type_info &info);

            const std::type_info &getInfo();

            /*
             * Get a name with c++ standard. Ex: std::string or int*
             */
            std::string getStandardName();

            /*
             * Type is primitive. Ex: int, float...
             */
            bool isPrimitive();

            /*
             * Type is an array.
             */
            bool isArray();

            /*
             * Type is a function.
             */
            bool isFunction();

            /*
             * Type is an enum
             */
            bool isEnum();

            /*
             * Type is a class
             */
            bool isClass();

            /*
             * Type is a class and only have a single public Base
             */
            bool isSimpleClass();

            /*
             * Type is a class and have more than one father or a single non public Base
             */
            bool isVirtalClass();

            /*
             * Type is a base class of the argument class
             */
            bool isBaseOf(const std::type_info &info);

            /*
             * Type is a derived class of the argument class
             */
            bool isDerivedOf(const std::type_info &info);

            /*
             * Get the number of base class
             */
            unsigned int getNumOfBases();

            /*
             * Get type of one of base classes
             * @param p position of base
             */
            const std::type_info &getBase(int p);

            /*
             * Get base class or the first if there are more than one
             */
            const std::type_info &getBase();

            /*
             * Type is a pointer
             */
            bool isPointer();

            /*
            * Type of pointed to class
            */
            const std::type_info &getClassPointed();

            /*
             * Type is a pointer to a class member
             */
            bool isMemberPointer();

            /*
             * Type of class of pointed to member
             */
            const std::type_info &getClassMemberPointed();
        };
    }
}


#endif //TEST1_RTTINFO_H
