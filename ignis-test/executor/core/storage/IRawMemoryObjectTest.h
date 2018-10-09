
#ifndef IGNIS_IRAWMEMORYOBJECTTEST_H
#define IGNIS_IRAWMEMORYOBJECTTEST_H

#include "IObjectTest.h"
#include "../../../../ignis/executor/core/storage/IRawMemoryObject.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {
                class IRawMemoryObjectTest : public IObjectTest {

                CPPUNIT_TEST_SUITE(IRawMemoryObjectTest);
                        CPPUNIT_TEST(itWriteItReadTest);
                        CPPUNIT_TEST(itWriteTransReadTest);
                        CPPUNIT_TEST(transWriteItReadTest);
                        CPPUNIT_TEST(transWriteTransReadTest);
                        CPPUNIT_TEST(clearTest);
                        CPPUNIT_TEST(appendTest);
                        CPPUNIT_TEST(copyTest);
                        CPPUNIT_TEST(moveTest);
                    CPPUNIT_TEST_SUITE_END();

                    virtual std::shared_ptr<IObject> getObject(std::shared_ptr<api::IManager<IObject::Any>> m,
                                                               size_t lines, size_t bytes) {
                        auto object = std::make_shared<IRawMemoryObject>(6, bytes);
                        object->setManager(m);
                        return object;
                    }

                };
            }
        }
    }
}

#endif
