
#ifndef IGNIS_IPARTITIONTEST_H
#define IGNIS_IPARTITIONTEST_H

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include "ignis/executor/core/storage/IPartition.h"
#include "ignis/executor/core/IElements.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {
                using api::IVector;

                template<typename Tp>
                class IPartitionTest : public CPPUNIT_NS::TestCase {
                public:

                    void setUp();

                    void itWriteItReadTest();

                    void itWriteTransReadTest();

                    void transWriteItReadTest();

                    void transWriteTransReadTest();

                    void clearTest();

                    void appendItWriteTest();

                    void appendTransReadTest();

                    void copyTest();

                    void moveTest();

                    void tearDown();

                protected:
                    virtual std::shared_ptr<IPartition<Tp>> create() = 0;

                    template<typename Tp2>
                    IVector<Tp2> elements(int n, int seed);

                    // from ... to ...
                    void writeIterator(IVector<Tp> &elems, IPartition<Tp> &part);

                    void readIterator(IPartition<Tp> &part, IVector<Tp> &elems);

                    void read(IVector<Tp> &elems, IPartition<Tp> &part);

                    void write(IPartition<Tp> &part, IVector<Tp> &elems);

                };
            }
        }
    }
}

#include "IPartitionTest.tcc"

#endif

