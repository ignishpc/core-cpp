
#include "IObjectTest.h"
#include <vector>
#include <thrift/transport/TBufferTransports.h>
#include "../../../../ignis/data/IZlibTransport.h"
#include "../../../../ignis/data/IObjectProtocol.h"

using namespace ignis::executor::core::storage;
using namespace apache::thrift::transport;

void IObjectTest::setUp() {
    auto manager = std::make_shared<api::IManager<std::string>>();
    auto manager_any = (std::shared_ptr<api::IManager<IObject::Any>> &) manager;
    object = getObject(manager_any, 100, 10 * 1024);
}

void IObjectTest::itWriteItReadTest() {
    std::srand(0);
    std::vector<std::string> examples;
    for (int i = 0; i < 100; i++) {
        examples.push_back(std::to_string(std::rand() % 100));
    }
//////////////////////////////////////////////////////////////////////////
    auto write = object->writeIterator();
    for (auto &elem: examples) {
        write->write((IObject::Any &) elem);
    }
    CPPUNIT_ASSERT_EQUAL(examples.size(), object->getSize());
//////////////////////////////////////////////////////////////////////////
    auto reader = object->readIterator();
    for (auto &elem: examples) {
        CPPUNIT_ASSERT(reader->hasNext());
        CPPUNIT_ASSERT_EQUAL(elem, (std::string &) reader->next());
    }
    CPPUNIT_ASSERT(!reader->hasNext());
}

void IObjectTest::itWriteTransReadTest() {
    std::srand(0);
    std::vector<std::string> examples;
    for (int i = 0; i < 100; i++) {
        examples.push_back(std::to_string(std::rand() % 100));
    }
//////////////////////////////////////////////////////////////////////////
    auto write = object->writeIterator();
    for (auto &elem: examples) {
        write->write((IObject::Any &) elem);
    }
    CPPUNIT_ASSERT_EQUAL(examples.size(), object->getSize());
//////////////////////////////////////////////////////////////////////////
    auto w_buffer = std::make_shared<transport::TMemoryBuffer>();
    object->write(w_buffer, 6);

    auto w_transport = std::make_shared<data::IZlibTransport>(w_buffer);
    auto w_protocol = std::make_shared<data::IObjectProtocol>(w_transport);
    data::handle::IReader<std::vector<std::string>> reader;
    std::shared_ptr<std::vector<std::string>> result = w_protocol->readObject(reader);

    CPPUNIT_ASSERT_EQUAL(examples.size(), result->size());
    for (int i = 0; i < examples.size(); i++) {
        CPPUNIT_ASSERT_EQUAL(examples[i], (*result)[i]);
    }
}

void IObjectTest::transWriteItReadTest() {
    std::srand(0);
    std::vector<std::string> examples;
    for (int i = 0; i < 100; i++) {
        examples.push_back(std::to_string(std::rand() % 100));
    }
//////////////////////////////////////////////////////////////////////////
    auto r_buffer = std::make_shared<transport::TMemoryBuffer>();
    auto r_transport = std::make_shared<data::IZlibTransport>(r_buffer, 6);
    auto r_protocol = std::make_shared<data::IObjectProtocol>(r_transport);
    data::handle::IWriter<std::vector<std::string>> writer;
    r_protocol->writeObject(examples, writer);
    r_transport->flush();
    object->read(r_buffer);
    CPPUNIT_ASSERT_EQUAL(examples.size(), object->getSize());
//////////////////////////////////////////////////////////////////////////
    auto reader = object->readIterator();
    for (auto &elem: examples) {
        CPPUNIT_ASSERT(reader->hasNext());
        CPPUNIT_ASSERT_EQUAL(elem, (std::string &) reader->next());
    }
    CPPUNIT_ASSERT(!reader->hasNext());
}

void IObjectTest::transWriteTransReadTest() {
    std::srand(0);
    std::vector<std::string> examples;
    for (int i = 0; i < 100; i++) {
        examples.push_back(std::to_string(std::rand() % 100));
    }
//////////////////////////////////////////////////////////////////////////
    auto r_buffer = std::make_shared<transport::TMemoryBuffer>();
    auto r_transport = std::make_shared<data::IZlibTransport>(r_buffer, 6);
    auto r_protocol = std::make_shared<data::IObjectProtocol>(r_transport);
    data::handle::IWriter<std::vector<std::string>> writer;
    r_protocol->writeObject(examples, writer);
    r_transport->flush();
    object->read(r_buffer);
    CPPUNIT_ASSERT_EQUAL(examples.size(), object->getSize());
//////////////////////////////////////////////////////////////////////////
    auto w_buffer = std::make_shared<transport::TMemoryBuffer>();
    object->write(w_buffer, 6);

    auto w_transport = std::make_shared<data::IZlibTransport>(w_buffer);
    auto w_protocol = std::make_shared<data::IObjectProtocol>(w_transport);
    data::handle::IReader<std::vector<std::string>> reader;
    std::shared_ptr<std::vector<std::string>> result = w_protocol->readObject(reader);

    CPPUNIT_ASSERT_EQUAL(examples.size(), result->size());
    for (int i = 0; i < examples.size(); i++) {
        CPPUNIT_ASSERT_EQUAL(examples[i], (*result)[i]);
    }
}

void IObjectTest::clearTest() {
    itWriteItReadTest();
    long size = object->getSize();
    object->clear();
    CPPUNIT_ASSERT_EQUAL(0ul, object->getSize());
    itWriteItReadTest();
}

void IObjectTest::appendTest() {
    std::srand(0);
    std::vector<std::string> examples;
    std::vector<std::string> examples2;

    for (int i = 0; i < 100; i++) {
        examples.push_back(std::to_string(std::rand() % 100));
        examples2.push_back(std::to_string(std::rand() % 100));
    }

//////////////////////////////////////////////////////////////////////////
    auto writer = object->writeIterator();
    for (auto &elem: examples) {
        writer->write((IObject::Any &) elem);
    }
    CPPUNIT_ASSERT_EQUAL(examples.size(), object->getSize());
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
    writer = object->writeIterator();
    for (auto &elem: examples2) {
        writer->write((IObject::Any &) elem);
    }
    CPPUNIT_ASSERT_EQUAL(examples.size() + examples2.size(), object->getSize());
//////////////////////////////////////////////////////////////////////////

    auto reader = object->readIterator();
    for (auto &elem: examples) {
        CPPUNIT_ASSERT(reader->hasNext());
        CPPUNIT_ASSERT_EQUAL(elem, (std::string &) reader->next());
    }
    for (auto &elem: examples2) {
        CPPUNIT_ASSERT(reader->hasNext());
        CPPUNIT_ASSERT_EQUAL(elem, (std::string &) reader->next());
    }
    CPPUNIT_ASSERT(!reader->hasNext());
}

void IObjectTest::copyTest() {
    std::srand(0);
    std::vector<std::string> examples;
    for (int i = 0; i < 100; i++) {
        examples.push_back(std::to_string(std::rand() % 100));
    }
//////////////////////////////////////////////////////////////////////////
    auto writer = object->writeIterator();
    for (auto &elem: examples) {
        writer->write((IObject::Any &) elem);
    }
    CPPUNIT_ASSERT_EQUAL(examples.size(), object->getSize());
//////////////////////////////////////////////////////////////////////////
    auto copy = getObject(object->getManager(), 100, 10 * 1024);
    copy->copyFrom(*object);
    copy->copyFrom(*object);
    CPPUNIT_ASSERT_EQUAL(examples.size(), object->getSize());
    CPPUNIT_ASSERT_EQUAL(examples.size() * 2, copy->getSize());
//////////////////////////////////////////////////////////////////////////
    auto reader = object->readIterator();
    auto reader_copy = copy->readIterator();
    for (auto &elem: examples) {
        CPPUNIT_ASSERT(reader->hasNext());
        CPPUNIT_ASSERT_EQUAL(elem, (std::string &) reader->next());
        CPPUNIT_ASSERT(reader_copy->hasNext());
        CPPUNIT_ASSERT_EQUAL(elem, (std::string &) reader_copy->next());
    }
    for (auto &elem: examples) {
        CPPUNIT_ASSERT(reader_copy->hasNext());
        CPPUNIT_ASSERT_EQUAL(elem, (std::string &) reader_copy->next());
    }
    CPPUNIT_ASSERT(!reader->hasNext());
    CPPUNIT_ASSERT(!reader_copy->hasNext());
}

void IObjectTest::moveTest() {
    std::srand(0);
    std::vector<std::string> examples;
    for (int i = 0; i < 100; i++) {
        examples.push_back(std::to_string(std::rand() % 100));
    }
//////////////////////////////////////////////////////////////////////////
    auto writer = object->writeIterator();
    for (auto &elem: examples) {
        writer->write((IObject::Any &) elem);
    }
    CPPUNIT_ASSERT_EQUAL(examples.size(), object->getSize());
//////////////////////////////////////////////////////////////////////////
    auto moved = getObject(object->getManager(), 100, 10 * 1024);
    moved->moveFrom(*object);
    CPPUNIT_ASSERT_EQUAL(0ul, object->getSize());
    CPPUNIT_ASSERT_EQUAL(examples.size(), moved->getSize());
    object = moved;

//////////////////////////////////////////////////////////////////////////
    auto reader = object->readIterator();
    for (auto &elem: examples) {
        CPPUNIT_ASSERT(reader->hasNext());
        CPPUNIT_ASSERT_EQUAL(elem, (std::string &) reader->next());
    }
    CPPUNIT_ASSERT(!reader->hasNext());
}

void IObjectTest::skipTest() {
    std::srand(0);
    std::vector<std::string> examples;
    for (int i = 0; i < 100; i++) {
        examples.push_back(std::to_string(std::rand() % 100));
    }
//////////////////////////////////////////////////////////////////////////
    auto write = object->writeIterator();
    for (auto &elem: examples) {
        write->write((IObject::Any &) elem);
    }
    CPPUNIT_ASSERT_EQUAL(examples.size(), object->getSize());
//////////////////////////////////////////////////////////////////////////
    auto reader = object->readIterator();
    size_t skipped = object->getSize() / 2;
    reader->skip(skipped);
    for (size_t i = skipped; i < examples.size(); i++) {
        CPPUNIT_ASSERT(reader->hasNext());
        CPPUNIT_ASSERT_EQUAL(examples[i], (std::string &) reader->next());
    }
    CPPUNIT_ASSERT(!reader->hasNext());
}

void IObjectTest::tearDown() {
    object->clear();
    object.reset();
}