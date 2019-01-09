
#ifndef IGNIS_IMAPPERMODULETEST_H
#define IGNIS_IMAPPERMODULETEST_H

#include "IModuleTest.h"
#include "../../../../ignis/executor/core/modules/IMapperModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IMapperModuleTest : public IModuleTest {

                CPPUNIT_TEST_SUITE(IMapperModuleTest);
                        CPPUNIT_TEST(sequentialMap);
                        CPPUNIT_TEST(sequentialFlatMap);
                        CPPUNIT_TEST(sequentialFilter);
                        CPPUNIT_TEST(sequentialKeyBy);
                        CPPUNIT_TEST(parallelMap);
                        CPPUNIT_TEST(parallelFlatMap);
                        CPPUNIT_TEST(parallelFilter);
                        CPPUNIT_TEST(parallelKeyBy);
                        CPPUNIT_TEST(sequentialStreamingMap);
                        CPPUNIT_TEST(sequentialStreamingFlatMap);
                        CPPUNIT_TEST(sequentialStreamingFilter);
                        CPPUNIT_TEST(sequentialStreamingKeyBy);
                        CPPUNIT_TEST(parallelStreamingMap);
                        CPPUNIT_TEST(parallelStreamingFlatMap);
                        CPPUNIT_TEST(parallelStreamingFilter);
                        CPPUNIT_TEST(parallelStreamingKeyBy);
                        CPPUNIT_TEST(parallelStreamingMapOrdered);
                        CPPUNIT_TEST(parallelStreamingFlatMapOrdered);
                        CPPUNIT_TEST(parallelStreamingFilterOrdered);
                        CPPUNIT_TEST(parallelStreamingKeyByOrdered);
                        CPPUNIT_TEST(mapPartition);
                        CPPUNIT_TEST(mapPartitionWithIndex);
                    CPPUNIT_TEST_SUITE_END();
                public:
                    virtual void setUp();

                    virtual void sequentialMap();

                    virtual void sequentialFlatMap();

                    virtual void sequentialFilter();

                    virtual void sequentialKeyBy();

                    virtual void parallelMap();

                    virtual void parallelFlatMap();

                    virtual void parallelFilter();

                    virtual void parallelKeyBy();

                    virtual void sequentialStreamingMap();

                    virtual void sequentialStreamingFlatMap();

                    virtual void sequentialStreamingFilter();

                    virtual void sequentialStreamingKeyBy();

                    virtual void parallelStreamingMap();

                    virtual void parallelStreamingFlatMap();

                    virtual void parallelStreamingFilter();

                    virtual void parallelStreamingKeyBy();

                    virtual void parallelStreamingMapOrdered();

                    virtual void parallelStreamingFlatMapOrdered();

                    virtual void parallelStreamingFilterOrdered();

                    virtual void parallelStreamingKeyByOrdered();

                    virtual void mapPartition();

                    virtual void mapPartitionWithIndex();

                    virtual void tearDown();

                private:

                    std::vector<int> input;
                    std::shared_ptr<IMapperModule> mapper_module;

                };
            }
        }
    }
}

#endif
