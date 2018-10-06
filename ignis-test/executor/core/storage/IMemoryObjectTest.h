
#ifndef IGNIS_IMEMORYOBJECTTEST_H
#define IGNIS_IMEMORYOBJECTTEST_H


#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

class IMemoryObjectTest : public CPPUNIT_NS::TestCase {

CPPUNIT_TEST_SUITE(IMemoryObjectTest);
        CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();

public:

    void setUp() {
        int b = 2;
    }

    void test() {
        int a = 2;
    }

    void tearDown() {};

};

CPPUNIT_TEST_SUITE_REGISTRATION(IMemoryObjectTest);

#endif
