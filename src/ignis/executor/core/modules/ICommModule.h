
#ifndef IGNIS_ICOMMMODULE_H
#define IGNIS_ICOMMMODULE_H

#include "IModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class ICommModule : public modules::IModule {
                public:

                    ICommModule(std::shared_ptr<IExecutorData> &executor_data);
/*
                    virtual void startServer(std::string &_return);

                    virtual void stopServer();

                    virtual void recvPartitions() {}//TODO

                    virtual void createGroup();

                    virtual void joinGroup(std::string& server);*/

                    virtual ~ICommModule();

                private:


                };
            }
        }
    }
}

#endif

