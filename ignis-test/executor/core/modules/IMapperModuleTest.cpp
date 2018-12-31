
#include "IMapperModuleTest.h"
#include <vector>

using namespace ignis::executor::core::modules;

void IMapperModuleTest::setUp() {
    executor_data->getContext()["ignis.executor.storage"] = "rawMemory";
    executor_data->getContext()["ignis.executor.storage.compression"] = "6";
    executor_data->getContext()["ignis.executor.cores.chunk"] = "5";
    mapper_module = std::make_shared<IMapperModule>(executor_data);
    auto manager = std::make_shared<api::IManager<int>>();
    executor_data->
            loadObject(mapper_module->getIObject((std::shared_ptr<api::IManager<storage::IObject::Any>> &) manager));

    std::srand(0);
    auto writer = executor_data->loadObject()->writeIterator();

    for (int i = 0; i < 1001; i++) {
        int value = std::rand() % 100;
        input.push_back(value);
        writer->write((storage::IObject::Any &) value);
    }
}

void IMapperModuleTest::tearDown() {

}

void IMapperModuleTest::sequentialMap() {
    executor_data->getContext()["ignis.executor.cores"] = "1";
    rpc::ISource f;
    f.__set_name(library + ":mapFunction");
    mapper_module->_map(f);

    auto reader = executor_data->loadObject()->readIterator();

    for (auto &elem:input) {
        CPPUNIT_ASSERT_EQUAL(std::to_string(elem), (std::string &) reader->next());
    }
}

void IMapperModuleTest::sequentialFlatMap() {
    executor_data->getContext()["ignis.executor.cores"] = "1";
    rpc::ISource f;
    f.__set_name(library + ":flatmapFunction");
    mapper_module->flatmap(f);

    auto reader = executor_data->loadObject()->readIterator();

    for (auto &elem:input) {
        if (elem % 2 == 0) {
            CPPUNIT_ASSERT_EQUAL(std::to_string(elem), (std::string &) reader->next());
        }
    }
}

void IMapperModuleTest::sequentialFilter() {
    executor_data->getContext()["ignis.executor.cores"] = "1";
    rpc::ISource f;
    f.__set_name(library + ":filterFunction");
    mapper_module->filter(f);

    auto reader = executor_data->loadObject()->readIterator();

    for (auto &elem:input) {
        if (elem % 2 == 0) {
            CPPUNIT_ASSERT_EQUAL(elem, (int &) reader->next());
        }
    }

}

void IMapperModuleTest::sequentialKeyBy() {
    executor_data->getContext()["ignis.executor.cores"] = "1";
    rpc::ISource f;
    f.__set_name(library + ":mapFunction");
    mapper_module->keyBy(f);

    auto reader = executor_data->loadObject()->readIterator();

    for (auto &elem:input) {
        auto &next = (std::pair<std::string, int> &) reader->next();
        CPPUNIT_ASSERT_EQUAL(elem, next.second);
        CPPUNIT_ASSERT_EQUAL(std::to_string(elem), next.first);
    }
}

void IMapperModuleTest::parallelMap() {
    executor_data->getContext()["ignis.executor.cores"] = "8";
    rpc::ISource f;
    f.__set_name(library + ":mapFunction");
    mapper_module->_map(f);

    auto reader = executor_data->loadObject()->readIterator();

    for (auto &elem:input) {
        CPPUNIT_ASSERT_EQUAL(std::to_string(elem), (std::string &) reader->next());
    }
}

void IMapperModuleTest::parallelFlatMap() {
    executor_data->getContext()["ignis.executor.cores"] = "8";
    rpc::ISource f;
    f.__set_name(library + ":flatmapFunction");
    mapper_module->flatmap(f);

    auto reader = executor_data->loadObject()->readIterator();

    for (auto &elem:input) {
        if (elem % 2 == 0) {
            CPPUNIT_ASSERT_EQUAL(std::to_string(elem), (std::string &) reader->next());
        }
    }
}

void IMapperModuleTest::parallelFilter() {
    executor_data->getContext()["ignis.executor.cores"] = "8";
    rpc::ISource f;
    f.__set_name(library + ":filterFunction");
    mapper_module->filter(f);

    auto reader = executor_data->loadObject()->readIterator();

    for (auto &elem:input) {
        if (elem % 2 == 0) {
            CPPUNIT_ASSERT_EQUAL(elem, (int &) reader->next());
        }
    }
}

void IMapperModuleTest::parallelKeyBy() {
    executor_data->getContext()["ignis.executor.cores"] = "8";
    rpc::ISource f;
    f.__set_name(library + ":mapFunction");
    mapper_module->keyBy(f);

    auto reader = executor_data->loadObject()->readIterator();

    for (auto &elem:input) {
        auto &next = (std::pair<std::string, int> &) reader->next();
        CPPUNIT_ASSERT_EQUAL(elem, next.second);
        CPPUNIT_ASSERT_EQUAL(std::to_string(elem), next.first);
    }
}

void IMapperModuleTest::sequentialStreamingMap() {
    executor_data->getContext()["ignis.executor.cores"] = "1";
    rpc::ISource f;
    f.__set_name(library + ":mapFunction");
    mapper_module->streamingMap(f, true);

    auto reader = executor_data->loadObject()->readIterator();

    for (auto &elem:input) {
        CPPUNIT_ASSERT_EQUAL(std::to_string(elem), (std::string &) reader->next());
    }
}

void IMapperModuleTest::sequentialStreamingFlatMap() {
    executor_data->getContext()["ignis.executor.cores"] = "1";
    rpc::ISource f;
    f.__set_name(library + ":flatmapFunction");
    mapper_module->streamingFlatmap(f, true);

    auto reader = executor_data->loadObject()->readIterator();

    for (auto &elem:input) {
        if (elem % 2 == 0) {
            CPPUNIT_ASSERT_EQUAL(std::to_string(elem), (std::string &) reader->next());
        }
    }
}

void IMapperModuleTest::sequentialStreamingFilter() {
    executor_data->getContext()["ignis.executor.cores"] = "1";
    rpc::ISource f;
    f.__set_name(library + ":filterFunction");
    mapper_module->streamingFilter(f, true);

    auto reader = executor_data->loadObject()->readIterator();

    for (auto &elem:input) {
        if (elem % 2 == 0) {
            CPPUNIT_ASSERT_EQUAL(elem, (int &) reader->next());
        }
    }
}

void IMapperModuleTest::sequentialStreamingKeyBy() {
    executor_data->getContext()["ignis.executor.cores"] = "1";
    rpc::ISource f;
    f.__set_name(library + ":mapFunction");
    mapper_module->streamingKeyBy(f, true);

    auto reader = executor_data->loadObject()->readIterator();

    for (auto &elem:input) {
        auto &next = (std::pair<std::string, int> &) reader->next();
        CPPUNIT_ASSERT_EQUAL(elem, next.second);
        CPPUNIT_ASSERT_EQUAL(std::to_string(elem), next.first);
    }
}

void IMapperModuleTest::parallelStreamingMap() {
    executor_data->getContext()["ignis.executor.cores"] = "8";
    rpc::ISource f;
    f.__set_name(library + ":mapFunction");
    mapper_module->streamingMap(f, false);

    auto reader = executor_data->loadObject()->readIterator();

    std::unordered_map<std::string, int> counts;

    for (auto &elem:input) {
        counts[std::to_string(elem)]++;
        counts[(std::string &) reader->next()]--;
    }

    for (auto &count:counts) {
        CPPUNIT_ASSERT_EQUAL(0, count.second);
    }
}

void IMapperModuleTest::parallelStreamingFlatMap() {
    executor_data->getContext()["ignis.executor.cores"] = "8";
    rpc::ISource f;
    f.__set_name(library + ":flatmapFunction");
    mapper_module->streamingFlatmap(f, false);

    auto reader = executor_data->loadObject()->readIterator();

    std::unordered_map<std::string, int> counts;

    for (auto &elem:input) {
        if (elem % 2 == 0) {
            counts[std::to_string(elem)]++;
            counts[(std::string &) reader->next()]--;
        }
    }

    for (auto &count:counts) {
        CPPUNIT_ASSERT_EQUAL(0, count.second);
    }
}

void IMapperModuleTest::parallelStreamingFilter() {
    executor_data->getContext()["ignis.executor.cores"] = "8";
    rpc::ISource f;
    f.__set_name(library + ":filterFunction");
    mapper_module->streamingFilter(f, false);

    auto reader = executor_data->loadObject()->readIterator();

    std::unordered_map<std::string, int> counts;

    for (auto &elem:input) {
        if (elem % 2 == 0) {
            counts[std::to_string(elem)]++;
            counts[std::to_string((int &) reader->next())]--;
        }
    }

    for (auto &count:counts) {
        CPPUNIT_ASSERT_EQUAL(0, count.second);
    }
}

void IMapperModuleTest::parallelStreamingKeyBy() {
    executor_data->getContext()["ignis.executor.cores"] = "8";
    rpc::ISource f;
    f.__set_name(library + ":mapFunction");
    mapper_module->streamingKeyBy(f, false);

    auto reader = executor_data->loadObject()->readIterator();

    std::unordered_map<std::string, int> counts;

    for (auto &elem:input) {
        counts[std::to_string(elem)]++;
        auto &next = (std::pair<std::string, int> &) reader->next();
        CPPUNIT_ASSERT_EQUAL(std::to_string(next.second), next.first);
        counts[next.first]--;
    }

    for (auto &count:counts) {
        CPPUNIT_ASSERT_EQUAL(0, count.second);
    }
}

void IMapperModuleTest::parallelStreamingMapOrdered() {
    executor_data->getContext()["ignis.executor.cores"] = "8";
    rpc::ISource f;
    f.__set_name(library + ":mapFunction");
    mapper_module->streamingMap(f, true);

    auto reader = executor_data->loadObject()->readIterator();

    for (auto &elem:input) {
        CPPUNIT_ASSERT_EQUAL(std::to_string(elem), (std::string &) reader->next());
    }
}

void IMapperModuleTest::parallelStreamingFlatMapOrdered() {
    executor_data->getContext()["ignis.executor.cores"] = "8";
    rpc::ISource f;
    f.__set_name(library + ":flatmapFunction");
    mapper_module->streamingFlatmap(f, true);

    auto reader = executor_data->loadObject()->readIterator();

    for (auto &elem:input) {
        if (elem % 2 == 0) {
            CPPUNIT_ASSERT_EQUAL(std::to_string(elem), (std::string &) reader->next());
        }
    }
}

void IMapperModuleTest::parallelStreamingFilterOrdered() {
    executor_data->getContext()["ignis.executor.cores"] = "8";
    rpc::ISource f;
    f.__set_name(library + ":filterFunction");
    mapper_module->streamingFilter(f, true);

    auto reader = executor_data->loadObject()->readIterator();

    for (auto &elem:input) {
        if (elem % 2 == 0) {
            CPPUNIT_ASSERT_EQUAL(elem, (int &) reader->next());
        }
    }
}

void IMapperModuleTest::parallelStreamingKeyByOrdered() {
    executor_data->getContext()["ignis.executor.cores"] = "8";
    rpc::ISource f;
    f.__set_name(library + ":mapFunction");
    mapper_module->streamingKeyBy(f, true);

    auto reader = executor_data->loadObject()->readIterator();

    for (auto &elem:input) {
        auto &next = (std::pair<std::string, int> &) reader->next();
        CPPUNIT_ASSERT_EQUAL(elem, next.second);
        CPPUNIT_ASSERT_EQUAL(std::to_string(elem), next.first);
    }
}
