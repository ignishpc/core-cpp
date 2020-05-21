
#ifndef IGNIS_IMPITEST_H
#define IGNIS_IMPITEST_H

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include "ignis/executor/core/IElements.h"
#include "ignis/executor/core/storage/IMemoryPartition.h"
#include "ignis/executor/core/storage/IRawMemoryPartition.h"
#include "ignis/executor/core/storage/IDiskPartition.h"
#include "ignis/executor/core/IExecutorData.h"


namespace ignis {
    namespace executor {
        namespace core {

            template<typename Ps>
            class IMpiTest : public CPPUNIT_NS::TestCase {
            CPPUNIT_TEST_SUITE(IMpiTest<Ps>);
                    CPPUNIT_TEST(gather0Test);
                    CPPUNIT_TEST(gather1Test);
                    CPPUNIT_TEST(bcastTest);
                    CPPUNIT_TEST(sendRcvTest);
                    //CPPUNIT_TEST(sendRcvGroupToMemoryTest);
                    //CPPUNIT_TEST(sendRcvGroupToRawMemoryTest);
                    //CPPUNIT_TEST(sendRcvGroupToDiskTest);
                    //CPPUNIT_TEST(driverGather);
                    //CPPUNIT_TEST(driverScatter);
                    //CPPUNIT_TEST(driverScatterVoid);
                CPPUNIT_TEST_SUITE_END();
            public:
                typedef typename Ps::value_type Tp;

                void setUp();

                void gather0Test() { gatherTest(0); }

                void gather1Test() { gatherTest(1); }

                void bcastTest();

                void sendRcvTest();

                void sendRcvGroupToMemoryTest() { sendRcvGroupTest("Memory"); }

                void sendRcvGroupToRawMemoryTest() { sendRcvGroupTest("RawMemory"); }

                void sendRcvGroupToDiskTest() { sendRcvGroupTest("Disk"); }

                void driverGather();

                void driverScatter();

                void driverScatterVoid();

                void tearDown();

            private:
                void gatherTest(int root);

                void sendRcvGroupTest(const std::string &partitionType);

                void insert(api::IVector<Tp> &v, storage::IPartition<Tp> &part);

                void get(api::IVector<Tp> &v, storage::IPartition<Tp> &part);

                std::shared_ptr<storage::IPartition<Tp>> create();

                std::shared_ptr<IExecutorData> executor_data;
                int disk_id = 0;
            };
        }
    }
}

#include "IMpiTest.tcc"

#endif
