
#ifndef IGNIS_IGNISCONTEXT_H
#define IGNIS_IGNISCONTEXT_H

#include "ignis/executor/core/IVariable.h"
#include <memory>
#include <mpi.h>
#include <vector>
#include <unordered_map>

namespace ignis {
    namespace executor {
        namespace core {
            class IExecutorData;
        }
        namespace api {
            class IContext {
            public:
                IContext();

                int threads();

                int executors();

                int executorId();

                int threadId();

                const MPI::Intracomm &mpiGroup();

                std::unordered_map<std::string, std::string> &props();

                template<typename Tp>
                Tp &var(const std::string &name);

                bool isVar(const std::string &name);

                bool rmVar(const std::string &name);

                virtual ~IContext();

            private:
                friend class core::IExecutorData;

                std::unordered_map<std::string, std::string> properties;
                std::unordered_map<std::string, std::shared_ptr<executor::core::IVariable>> variables;
                std::vector<MPI::Intracomm> mpi_thread_group;
            };
        }// namespace api
    }    // namespace executor
}// namespace ignis

#include "IContext.tcc"
#endif
