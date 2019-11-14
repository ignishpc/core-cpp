
#include "IDiskPartitionTest.h"

#define IDiskPartitionTestClass ignis::executor::core::storage::IDiskPartitionTest

template<typename Tp>
void IDiskPartitionTestClass<Tp>::renameTest() {
    std::string path = "./diskpartitionTest";
    std::string newPath = "./diskpartitionTestRename";
    auto part = std::make_shared<IDiskPartition < Tp>>
    (path);
    IVector <Tp> elems = IElements<Tp>::create(100, 0);
    this->writeIterator(elems, *part);
    part->rename(newPath);
    std::remove(path.c_str());
    CPPUNIT_ASSERT_EQUAL(elems.size(), part->size());
    IVector <Tp> result;
    this->readIterator(*part, result);
    CPPUNIT_ASSERT(elems == result);
}

template<typename Tp>
void IDiskPartitionTestClass<Tp>::persistTest() {
    std::string path = "./diskpartitionTest";
    auto part = std::make_shared<IDiskPartition<Tp>>(path,6,true,false);
    IVector <Tp> elems = IElements<Tp>::create(100, 0);
    this->writeIterator(elems, *part);
    part->persist(true);
    part->sync();
    part = std::make_shared<IDiskPartition<Tp>>(path, 6, false, true);
    CPPUNIT_ASSERT_EQUAL(elems.size(), part->size());
    IVector <Tp> result;
    this->readIterator(*part, result);
    CPPUNIT_ASSERT(elems == result);
}

#undef IDiskPartitionTestClass