
#ifndef IGNIS_IMODULETEST_H
#define IGNIS_IMODULETEST_H

#include <vector>
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../../../../ignis/executor/core/IExecutorData.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IModuleTest  : public CPPUNIT_NS::TestCase{
                public:

                    IModuleTest();

                protected:
                    std::string library;
                    std::shared_ptr<IExecutorData> executor_data;

                };
            }
        }
    }
}

#endif
