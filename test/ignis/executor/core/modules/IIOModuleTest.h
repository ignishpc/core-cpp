
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
                    CPPUNIT_TEST(saveAsTextFile);
                    CPPUNIT_TEST_SUITE_END();

                public:
                    IIOModuleTest();

                    void setUp() override;

                    void tearDown() override;

                    void textFile1() { textFileTest(1, 1); }

                    void textFileN() { textFileTest(8, 2); }

                    void saveAsTextFile() { saveAsTextFileTest(8, 2); }

                private:
                    void textFileTest(int n, int cores);

                    void saveAsTextFileTest(int n, int cores);

                    std::shared_ptr<IIOModule> io;
                };
            }// namespace modules
        }    // namespace core
    }        // namespace executor
}// namespace ignis

#include "IIOModuleTest.tcc"

#endif
