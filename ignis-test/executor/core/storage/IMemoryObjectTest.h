
#ifndef IGNIS_IMEMORYOBJECTTEST_H
#define IGNIS_IMEMORYOBJECTTEST_H

#include "IObjectTest.h"

//#include "../../../../ignis/executor/api/IManager.h"
//#include "../../../../ignis/executor/core/storage/IMemoryObject.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {
                class IMemoryObjectTest : public IObjectTest {

                CPPUNIT_TEST_SUITE(IMemoryObjectTest);
                        CPPUNIT_TEST(testIterator);
                    CPPUNIT_TEST_SUITE_END();

                    std::shared_ptr<IObject> createObject() {
                        //return std::make_shared<IMemoryObject>()
                    }

                    CPPUNIT_TEST_SUITE_REGISTRATION(IMemoryObjectTest);
                };
            }
        }
    }
}

#endif
