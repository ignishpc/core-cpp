
#ifndef IGNIS_IPOSTMANMODULETEST_H
#define IGNIS_IPOSTMANMODULETEST_H

#include "IModuleTest.h"
#include "../../../../ignis/executor/core/modules/IPostmanModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IPostmanModuleTest : public IModuleTest {
                CPPUNIT_TEST_SUITE(IPostmanModuleTest);
                        CPPUNIT_TEST(localTest);
                        CPPUNIT_TEST(socketTest);
                        CPPUNIT_TEST(sharedMemoryTest);
                    CPPUNIT_TEST_SUITE_END();
                public:

                    virtual void setUp();

                    virtual void localTest();

                    virtual void socketTest();

                    virtual void sharedMemoryTest();

                    virtual void tearDown();
                private:

                    void test(std::string addr);
                    std::shared_ptr<IPostmanModule> postman_module;
                };
            }
        }
    }
}

#endif
