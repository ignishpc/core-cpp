
#ifndef IGNIS_IMPITEST_H
#define IGNIS_IMPITEST_H

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include "executor/core/IElements.h"
#include "executor/core/storage/IMemoryPartition.h"
#include "executor/core/storage/IRawMemoryPartition.h"
#include "executor/core/storage/IDiskPartition.h"
#include "executor/core/IExecutorData.h"


namespace ignis {
    namespace executor {
        namespace core {

            template<typename Ps>
            class IMpiTest : public CPPUNIT_NS::TestCase {
            CPPUNIT_TEST_SUITE(IMpiTest<Ps>);
                    CPPUNIT_TEST(gatherTest);
                    CPPUNIT_TEST(bcastTest);
                CPPUNIT_TEST_SUITE_END();
            public:
                typedef typename Ps::value_type Tp;

                void setUp();

                void gatherTest();

                void bcastTest();

                void tearDown();

            private:
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
