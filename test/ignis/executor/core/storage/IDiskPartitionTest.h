
#ifndef IGNIS_IDISKPARTITIONTEST_H
#define IGNIS_IDISKPARTITIONTEST_H

#include "IPartitionTest.h"
#include "ignis/executor/core/storage/IDiskPartition.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {

                template<typename Tp>
                class IDiskPartitionTest : public IPartitionTest<Tp> {
                CPPUNIT_TEST_SUITE(IDiskPartitionTest<Tp>);
                        CPPUNIT_TEST(itWriteItReadTest);
                        CPPUNIT_TEST(itWriteTransReadTest);
                        CPPUNIT_TEST(transWriteItReadTest);
                        CPPUNIT_TEST(transWriteTransReadTest);
                        CPPUNIT_TEST(clearTest);
                        CPPUNIT_TEST(appendItWriteTest);
                        CPPUNIT_TEST(appendTransReadTest);
                        CPPUNIT_TEST(copyTest);
                        CPPUNIT_TEST(moveTest);
                        CPPUNIT_TEST(renameTest);
                        CPPUNIT_TEST(persistTest);
                    CPPUNIT_TEST_SUITE_END();
                public:
                    virtual void renameTest();

                    virtual void persistTest();

                private:
                    virtual std::shared_ptr<IPartition < Tp>> create() {
                        std::string path = "./diskpartitionTest" + std::to_string(file++);
                        return std::make_shared<IDiskPartition < Tp>>
                        (path);
                    }

                    int file = 0;
                };

            }
        }
    }
}

#include "IDiskPartitionTest.tcc"

#endif

