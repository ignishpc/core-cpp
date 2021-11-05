
#ifndef IGNIS_IMPITEST_H
#define IGNIS_IMPITEST_H

#include "ignis/executor/core/IElements.h"
#include "ignis/executor/core/IExecutorData.h"
#include "ignis/executor/core/storage/IDiskPartition.h"
#include "ignis/executor/core/storage/IMemoryPartition.h"
#include "ignis/executor/core/storage/IRawMemoryPartition.h"
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>


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
                CPPUNIT_TEST(sendRcvGroupToMemoryTest);
                CPPUNIT_TEST(sendRcvGroupToRawMemoryTest);
                CPPUNIT_TEST(sendRcvGroupToDiskTest);
                CPPUNIT_TEST(sendRcvGroupToVoidTest);
                CPPUNIT_TEST(driverGatherTest);
                CPPUNIT_TEST(driverScatterTest);
                CPPUNIT_TEST(driverScatterVoidTest);
                CPPUNIT_TEST(syncExchangeTest);
                CPPUNIT_TEST(syncExchangeCoresTest);
                CPPUNIT_TEST(asyncExchangeTest);
                CPPUNIT_TEST(asyncExchangeCoresTest);
                CPPUNIT_TEST(autoExchangeTest);
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

                void sendRcvGroupToVoidTest();

                void driverGatherTest();

                void driverScatterTest();

                void driverScatterVoidTest();

                void syncExchangeTest();

                void syncExchangeCoresTest();

                void asyncExchangeTest();

                void asyncExchangeCoresTest();

                void autoExchangeTest();

                void tearDown();

            private:
                void gatherTest(int root);

                void exchange();

                void sendRcvGroupTest(const std::string &partitionType);

                void insert(api::IVector<Tp> &v, storage::IPartition<Tp> &part);

                void get(api::IVector<Tp> &v, storage::IPartition<Tp> &part);

                std::shared_ptr<storage::IPartition<Tp>> create();

                std::shared_ptr<storage::IPartition<Tp>> create(const std::string &type);

                std::shared_ptr<IExecutorData> executor_data;
                int disk_id = 0;
            };
        }// namespace core
    }    // namespace executor
}// namespace ignis

#include "IMpiTest.tcc"

#endif
