
#ifndef INGIS_OBJECTLOADER_H
#define INGIS_OBJECTLOADER_H

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
                std::shared_ptr<selector::ISelectorGroup> loadFunction(const std::string &name);

                std::vector<std::string> loadLibrary(const std::string &path);
            };

        }// namespace core
    }    // namespace executor
}// namespace ignis
#endif
