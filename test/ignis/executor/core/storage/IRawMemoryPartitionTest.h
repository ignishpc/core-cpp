
#ifndef IGNIS_IRAWMEMORYPARTITIONTEST_H
#define IGNIS_IRAWMEMORYPARTITIONTEST_H

#include "IPartitionTest.h"
#include "ignis/executor/core/storage/IRawMemoryPartition.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {

                template<typename Tp>
                class IRawMemoryPartitionTest : public IPartitionTest<Tp> {
                    CPPUNIT_TEST_SUITE(IRawMemoryPartitionTest<Tp>);
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
                        return std::make_shared<IRawMemoryPartition<Tp>>();
                    }
                };
            }// namespace storage
        }    // namespace core
    }        // namespace executor
}// namespace ignis

#endif
