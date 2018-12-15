
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

                    virtual std::shared_ptr<IObject> getObject(std::shared_ptr<api::IManager<IObject::Any>> m,
                                                               size_t lines, size_t bytes) = 0;

                    virtual void setUp();

                    virtual void itWriteItReadTest();

                    virtual void itWriteTransReadTest();

                    virtual void transWriteItReadTest();

                    virtual void transWriteTransReadTest();

                    virtual void clearTest();

                    virtual void appendTest();

                    virtual void copyTest();

                    virtual void moveTest();

                    virtual void skipTest();

                    virtual void tearDown();

                private:

                    std::shared_ptr<IObject> object;

                };
            }
        }
    }
}

#endif
