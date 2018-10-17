
#ifndef IGNIS_IFILESMODULE_H
#define IGNIS_IFILESMODULE_H

#include "IModule.h"
#include "../../../rpc/executor/IFilesModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IFilesModule : public IgnisModule, public ignis::rpc::executor::IFilesModuleIf {
                public:
                    IFilesModule(std::shared_ptr<IExecutorData> &executor_data);

                    virtual void readFile(const std::string &path, const int64_t offset, const int64_t len,
                                          const int64_t lines) override;

                    virtual void saveFile(const std::string &path, const bool trunc, const bool new_line);

                    virtual void saveJson(const std::string &path, const bool array_start, const bool array_end);

                    virtual ~IFilesModule();

                };
            }
        }
    }
}

#endif
