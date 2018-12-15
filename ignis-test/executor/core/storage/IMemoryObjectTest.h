
#ifndef IGNIS_IMEMORYOBJECTTEST_H
#define IGNIS_IMEMORYOBJECTTEST_H

#include "IObjectTest.h"
#include "../../../../ignis/executor/core/storage/IMemoryObject.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {
                class IMemoryObjectTest : public IObjectTest {

                CPPUNIT_TEST_SUITE(IMemoryObjectTest);
                        CPPUNIT_TEST(itWriteItReadTest);
                        CPPUNIT_TEST(itWriteTransReadTest);
                        CPPUNIT_TEST(transWriteItReadTest);
                        CPPUNIT_TEST(transWriteTransReadTest);
                        CPPUNIT_TEST(clearTest);
                        CPPUNIT_TEST(appendTest);
                        CPPUNIT_TEST(copyTest);
                        CPPUNIT_TEST(moveTest);
                        CPPUNIT_TEST(skipTest);
                    CPPUNIT_TEST_SUITE_END();

                    virtual std::shared_ptr<IObject> getObject(std::shared_ptr<api::IManager<IObject::Any>> m,
                                                               size_t lines, size_t bytes) {
                        return std::make_shared<IMemoryObject>(m, lines);
                    }

                };
            }
        }
    }
}

#endif
