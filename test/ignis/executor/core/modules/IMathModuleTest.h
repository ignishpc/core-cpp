
#ifndef IGNIS_IMATHMODULETEST_H
#define IGNIS_IMATHMODULETEST_H

#include "IModuleTest.h"
#include "ignis/executor/core/modules/ImathModule.h"
namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IMathModuleTest : public IModuleTest {
                    CPPUNIT_TEST_SUITE(IMathModuleTest);
                    CPPUNIT_TEST(sampleWithReplacementIntTest);
                    CPPUNIT_TEST(sampleWithoutReplacementIntTest);
                    CPPUNIT_TEST(sampleByKeyIntTest);
                    CPPUNIT_TEST(countIntTest);
                    CPPUNIT_TEST(maxIntTest);
                    CPPUNIT_TEST(minIntTest);
                    CPPUNIT_TEST(countByKeyIntTest);
                    CPPUNIT_TEST(countByValueIntTest);
                    CPPUNIT_TEST_SUITE_END();

                public:
                    IMathModuleTest();

                    void setUp() override;

                    void tearDown() override;

                    void sampleWithReplacementIntTest() { sampleWithReplacementTest<int>(2, "RawMemory"); }

                    void sampleWithoutReplacementIntTest() { sampleWithoutReplacementTest<int>(2, "Memory"); }

                    void sampleByKeyIntTest() { sampleByKeyTest<int>(2, "Memory"); }

                    void countIntTest() { countTest<int>(2, "Memory"); }

                    void maxIntTest() { maxTest<int>(2, "Memory"); }

                    void minIntTest() { minTest<int>(2, "Memory"); }

                    void countByKeyIntTest() { countByKeyTest<int>(2, "Memory"); }

                    void countByValueIntTest() { countByValueTest<int>(2, "Memory"); }

                private:
                    template<typename Tp>
                    void sampleWithReplacementTest(int cores, const std::string &partitionType);

                    template<typename Tp>
                    void sampleWithoutReplacementTest(int cores, const std::string &partitionType);

                    template<typename Tp>
                    void sampleByKeyTest(int cores, const std::string &partitionType);

                    template<typename Tp>
                    void countTest(int cores, const std::string &partitionType);

                    template<typename Tp>
                    void maxTest(int cores, const std::string &partitionType);

                    template<typename Tp>
                    void minTest(int cores, const std::string &partitionType);

                    template<typename Tp>
                    void countByKeyTest(int cores, const std::string &partitionType);

                    template<typename Tp>
                    void countByValueTest(int cores, const std::string &partitionType);

                    std::shared_ptr<IMathModule> math;
                };
            }// namespace modules
        }    // namespace core
    }        // namespace executor
}// namespace ignis
#include "IMathModuleTest.tcc"

#endif//IGNIS_IMATHMODULETEST_H
