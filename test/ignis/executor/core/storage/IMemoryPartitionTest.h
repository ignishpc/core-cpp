
#ifndef IGNIS_IMEMORYPARTITIONTEST_H
#define IGNIS_IMEMORYPARTITIONTEST_H

#include "IPartitionTest.h"
#include "ignis/executor/core/storage/IMemoryPartition.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {

                template<typename Tp>
                class IMemoryPartitionTest : public IPartitionTest<Tp> {
                CPPUNIT_TEST_SUITE(IMemoryPartitionTest<Tp>);
                        CPPUNIT_TEST(itWriteItReadTest);
                        CPPUNIT_TEST(itWriteTransReadTest);
                        CPPUNIT_TEST(transWriteItReadTest);
                        CPPUNIT_TEST(transWriteTransReadTest);
                        CPPUNIT_TEST(clearTest);
                        CPPUNIT_TEST(appendItWriteTest);
                        CPPUNIT_TEST(appendTransReadTest);
                        CPPUNIT_TEST(copyTest);
                        CPPUNIT_TEST(moveTest);
                    CPPUNIT_TEST_SUITE_END();

                    virtual std::shared_ptr<IPartition<Tp>> create() {
                        return std::make_shared<IMemoryPartition<Tp>>();
                    }
                };
            }
        }
    }
}

#endif

