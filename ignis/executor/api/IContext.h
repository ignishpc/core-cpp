
#ifndef IGNIS_IGNISCONTEXT_H
#define IGNIS_IGNISCONTEXT_H

#include <unordered_map>
#include <memory>
#include <mpi.h>
#include "executor/core/protocol/IObjectProtocol.h"

namespace ignis {
    namespace executor {
        namespace core{
            class IExecutorData;
        }
        namespace api {
            class IContext {
            public:

                IContext();

                int cores();

                int executors();

                int executorId();

                int threadId();

                MPI::Intracomm& mpiGroup();

                std::unordered_map<std::string, std::string> &props();

                template<typename T>
                std::shared_ptr<T> &var(const std::string &name);

                bool rmVar(const std::string &name);

                virtual ~IContext();

            private:
                friend class core::IExecutorData;
                std::shared_ptr<core::protocol::IObjectProtocol> varProtocol(std::shared_ptr<void> &var);

                std::unordered_map<std::string, std::string> properties;
                std::unordered_map<std::string, std::pair<bool, std::shared_ptr<void>>> variables;
                MPI::Intracomm mpi_group;
            };
        }
    }
}

#include "IContext.tcc"
#endif
