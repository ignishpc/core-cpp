
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
                    CPPUNIT_TEST(voidTest);
                    CPPUNIT_TEST(textFile1Test);
                    CPPUNIT_TEST(textFileNTest);
                    CPPUNIT_TEST(saveAsTextFileTest);
                    CPPUNIT_TEST(partitionTextFileTest);
                    CPPUNIT_TEST(partitionJsonFileTest);
                    CPPUNIT_TEST(partitionJsonFileMapTest);
                    CPPUNIT_TEST(partitionObjectFileTest);
                    CPPUNIT_TEST_SUITE_END();

                public:
                    IIOModuleTest();

                    void setUp() override;

                    void tearDown() override;

                    void voidTest() {voidWithCompileTest(false);}

                    void textFile1Test() { textFileTest(1, 1); }

                    void textFileNTest() { textFileTest(8, 2); }

                    void saveAsTextFileTest() { saveAsTextFileTest(8, 2); }

                    void partitionTextFileTest();

                    void partitionJsonFileTest(){partitionJsonFileTestImpl(false);}

                    void partitionJsonFileMapTest(){partitionJsonFileTestImpl(true);}

                    void partitionObjectFileTest();

                private:
                    void voidWithCompileTest(bool compileTest);

                    void textFileTest(int n, int cores);

                    void saveAsTextFileTest(int n, int cores);

                    void partitionJsonFileTestImpl(bool objMap);

                    std::shared_ptr<IIOModule> io;
                };
            }// namespace modules
        }    // namespace core
    }        // namespace executor
}// namespace ignis

#include "IIOModuleTest.tcc"

#endif
