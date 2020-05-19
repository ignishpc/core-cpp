
#include "IPartitionTest.h"
#include "ignis/executor/core/protocol/IObjectProtocol.h"
#include "ignis/executor/core/transport/IMemoryBuffer.h"
#include "ignis/executor/core/transport/IZlibTransport.h"

#define IPartitionTestClass ignis::executor::core::storage::IPartitionTest

template<typename Tp>
void IPartitionTestClass<Tp>::writeIterator(IVector <Tp> &elems, IPartition <Tp> &part) {
    auto it = part.writeIterator();
    for (auto &e: elems) {
        it->write(e);
    }
}

template<typename Tp>
void IPartitionTestClass<Tp>::readIterator(IPartition <Tp> &part, IVector <Tp> &elems) {
    auto it = part.readIterator();
    while (it->hasNext()) {
        elems.push_back(it->next());
    }
}

template<typename Tp>
void IPartitionTestClass<Tp>::read(IVector <Tp> &elems, IPartition <Tp> &part) {
    auto buffer = std::make_shared<transport::IMemoryBuffer>();
    auto trans = std::make_shared<transport::IZlibTransport>(buffer, 9);
    protocol::IObjectProtocol(trans).writeObject(elems);
    trans->flush();
    part.read(reinterpret_cast<std::shared_ptr<transport::ITransport> &>(buffer));
}

template<typename Tp>
void IPartitionTestClass<Tp>::write(IPartition <Tp> &part, IVector <Tp> &elems) {
    auto buffer = std::make_shared<transport::IMemoryBuffer>();
    auto trans = std::make_shared<transport::IZlibTransport>(buffer);
    part.write(reinterpret_cast<std::shared_ptr<transport::ITransport> &>(buffer), 9);
    protocol::IObjectProtocol(trans).readObject(elems);
}

template<typename Tp>
void IPartitionTestClass<Tp>::setUp() {}

template<typename Tp>
void IPartitionTestClass<Tp>::tearDown() {}

template<typename Tp>
void IPartitionTestClass<Tp>::itWriteItReadTest() {
    auto part = create();
    IVector <Tp> elems = IElements<Tp>::create(100, 0);
    writeIterator(elems, *part);
    CPPUNIT_ASSERT_EQUAL(elems.size(), part->size());
    IVector <Tp> result;
    readIterator(*part, result);
    CPPUNIT_ASSERT(elems == result);
}


template<typename Tp>
void IPartitionTestClass<Tp>::itWriteTransReadTest() {
    auto part = create();
    IVector <Tp> elems = IElements<Tp>::create(100, 0);
    writeIterator(elems, *part);
    CPPUNIT_ASSERT_EQUAL(elems.size(), part->size());
    IVector <Tp> result;
    write(*part, result);
    CPPUNIT_ASSERT(elems == result);
}

template<typename Tp>
void IPartitionTestClass<Tp>::transWriteItReadTest() {
    auto part = create();
    IVector <Tp> elems = IElements<Tp>::create(100, 0);
    read(elems, *part);
    CPPUNIT_ASSERT_EQUAL(elems.size(), part->size());
    IVector <Tp> result;
    readIterator(*part, result);
    CPPUNIT_ASSERT(elems == result);
}

template<typename Tp>
void IPartitionTestClass<Tp>::transWriteTransReadTest() {
    auto part = create();
    IVector <Tp> elems = IElements<Tp>::create(100, 0);
    read(elems, *part);
    CPPUNIT_ASSERT_EQUAL(elems.size(), part->size());
    IVector <Tp> result;
    write(*part, result);
    CPPUNIT_ASSERT(elems == result);
}

template<typename Tp>
void IPartitionTestClass<Tp>::clearTest() {
    auto part = create();
    IVector <Tp> elems = IElements<Tp>::create(100, 0);
    IVector <Tp> result;
    read(elems, *part);
    part->clear();
    CPPUNIT_ASSERT_EQUAL((size_t) 0, part->size());
    read(elems, *part);
    write(*part, result);
    CPPUNIT_ASSERT(elems == result);
    part->clear();
    CPPUNIT_ASSERT_EQUAL((size_t) 0, part->size());
    writeIterator(elems, *part);
    result.clear();
    readIterator(*part, result);
    CPPUNIT_ASSERT(elems == result);
}

template<typename Tp>
void IPartitionTestClass<Tp>::appendItWriteTest() {
    auto part = create();
    IVector <Tp> elems = IElements<Tp>::create(100, 0);
    IVector <Tp> elems2 = IElements<Tp>::create(100, 1);
    writeIterator(elems, *part);
    writeIterator(elems2, *part);
    elems.insert(elems.end(), elems2.begin(), elems2.end());
    CPPUNIT_ASSERT_EQUAL(elems.size(), part->size());
    IVector <Tp> result;
    readIterator(*part, result);
    CPPUNIT_ASSERT(elems == result);
}

template<typename Tp>
void IPartitionTestClass<Tp>::appendTransReadTest() {
    auto part = create();
    IVector <Tp> elems = IElements<Tp>::create(100, 0);
    IVector <Tp> elems2 = IElements<Tp>::create(100, 1);
    read(elems, *part);
    read(elems2, *part);
    elems.insert(elems.end(), elems2.begin(), elems2.end());
    CPPUNIT_ASSERT_EQUAL(elems.size(), part->size());
    IVector <Tp> result;
    write(*part, result);
    CPPUNIT_ASSERT(elems == result);
}

template<typename Tp>
void IPartitionTestClass<Tp>::copyTest() {
    auto part = create();
    auto part2 = create();
    IVector <Tp> elems = IElements<Tp>::create(100, 0);
    IVector <Tp> elems2 = IElements<Tp>::create(100, 1);
    writeIterator(elems, *part);
    writeIterator(elems2, *part2);
    part2->copyTo(*part);
    elems.insert(elems.end(), elems2.begin(), elems2.end());
    CPPUNIT_ASSERT_EQUAL(elems.size(), part->size());
    CPPUNIT_ASSERT_EQUAL(elems2.size(), part2->size());
    IVector <Tp> result;
    readIterator(*part, result);
    CPPUNIT_ASSERT(elems == result);
}

template<typename Tp>
void IPartitionTestClass<Tp>::moveTest() {
    auto part = create();
    auto part2 = create();
    IVector <Tp> elems = IElements<Tp>::create(100, 0);
    IVector <Tp> elems2 = IElements<Tp>::create(100, 1);
    writeIterator(elems, *part);
    writeIterator(elems2, *part2);
    part2->moveTo(*part);
    elems.insert(elems.end(), elems2.begin(), elems2.end());
    CPPUNIT_ASSERT_EQUAL(elems.size(), part->size());
    CPPUNIT_ASSERT_EQUAL((size_t)0, part2->size());
    IVector <Tp> result;
    readIterator(*part, result);
    CPPUNIT_ASSERT(elems == result);
}




#undef IPartitionTestClass
