
#ifndef INGIS_OBJECTLOADER_H
#define INGIS_OBJECTLOADER_H

#include <memory>

namespace ignis {
    namespace executor {
        namespace core {
            class ILibraryLoader {
            public:
                template<typename T>
                std::shared_ptr<T> load(const std::string &name) {
                    return std::static_pointer_cast<T>(vload(name));
                }

            private:
                std::shared_ptr<void> vload(const std::string &name);
            };

        }// namespace core
    }    // namespace executor
}// namespace ignis
#endif
