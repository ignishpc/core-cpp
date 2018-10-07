
#ifndef IGNIS_IOBJECTTEST_H
#define IGNIS_IOBJECTTEST_H


#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../../../../ignis/executor/core/storage/IObject.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {
                class IObjectTest : public CPPUNIT_NS::TestCase {
                public:

                    virtual std::shared_ptr<IObject> createObject() = 0;

                    virtual void setUp();

                    virtual void testIterator();

                    virtual void tearDown();

                private:
                    std::shared_ptr<IObject> object;

                };
            }
        }
    }
}

#endif
