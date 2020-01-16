
#ifndef IGNIS_ICLUSTER_H
#define IGNIS_ICLUSTER_H

#include "IProperties.h"
#include <vector>

namespace ignis {
    namespace driver {
        namespace api {
            class IWorker;

            class ICluster {
            public:
                ICluster();

                ICluster(const std::string &name);

                ICluster(const std::shared_ptr<IProperties> &properties);

                ICluster(const std::string &name, const std::shared_ptr<IProperties> &properties);

                void setName(const std::string &name);

                void execute(const std::vector<std::string> &cmd);

                void execute(const std::string &cmd...);

                void executeScript(const std::string &script);

                void sendFile(const std::string &source, const std::string &target);

                void sendCompressedFile(const std::string &source, const std::string &target);

            private:
                friend IWorker;

                void execute(const std::initializer_list<std::string> &cmd);

                int64_t id;
            };
        }
    }
}

#endif
