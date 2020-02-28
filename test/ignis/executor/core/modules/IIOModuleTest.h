
#ifndef IGNIS_IIOMODULETEST_H
#define IGNIS_IIOMODULETEST_H

#include "IModuleTest.h"
#include "ignis/executor/core/modules/IIOModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IIOModuleTest : public IModuleTest {
                CPPUNIT_TEST_SUITE(IIOModuleTest);
                        CPPUNIT_TEST(textFile1);
                        CPPUNIT_TEST(textFileN);
                    CPPUNIT_TEST_SUITE_END();
                public:
                    IIOModuleTest();

                    void setUp() override;

                    void tearDown() override;

                    void textFile1() { textFileTest(1); }

                    void textFileN() { textFileTest(8); }

                private:

                    void textFileTest(int n);

                    std::shared_ptr<IIOModule> io;

                };
            }
        }
    }
}

#include "IIOModuleTest.tcc"

#endif
