
#ifndef INGIS_OBJECTLOADER_H
#define INGIS_OBJECTLOADER_H

#include "IPropertyParser.h"
#include <memory>
#include <vector>


namespace ignis {
    namespace executor {
        namespace core {
            namespace selector {
                class ISelectorGroup;
            }
            class ILibraryLoader {
            public:
                ILibraryLoader(IPropertyParser& properties);

                std::shared_ptr<selector::ISelectorGroup> loadFunction(const std::string &name);

                std::vector<std::string> loadLibrary(const std::string &path);

            private:
                IPropertyParser& properties;

                std::string compile(const std::string &str);

                std::string loadLambda(const std::string &str);

                std::string loadSource(const std::string &str);

            };

        }// namespace core
    }    // namespace executor
}// namespace ignis
#endif
