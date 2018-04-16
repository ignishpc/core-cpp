
#ifndef IGNIS_IFILESMODULE_H
#define IGNIS_IFILESMODULE_H

#include "IModule.h"
#include "../../../rpc/executor/IFilesModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IFilesModule : public IgnisModule, public ignis::rpc::executor::IFilesModuleIf{
                public:
                    IFilesModule(std::shared_ptr<IExecutorData> &executor_data);

                    void
                    readFile(const int64_t data_id, const std::string &path, const int64_t offset, const int64_t len,
                             const int64_t lines) override;

                    void saveFile(const int64_t data_id, const std::string &path, const bool joined) override;

                    void saveJson(const int64_t data_id, const std::string &path) override;

                    virtual ~IFilesModule();

                };
            }
        }
    }
}

#endif
