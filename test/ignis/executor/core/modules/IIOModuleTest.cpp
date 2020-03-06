
#include "IIOModuleTest.h"
#include <fstream>

using namespace ignis::executor::core::modules;

IIOModuleTest::IIOModuleTest() : io(std::make_shared<IIOModule>(executor_data)) {
    auto &props = executor_data->getContext().props();
    props["ignis.partition.minimal"] = "10MB";
    props["ignis.partition.type"] = "Memory";
}

void IIOModuleTest::setUp() {}

void IIOModuleTest::tearDown() {}

void IIOModuleTest::textFileTest(int n) {
    srand(0);
    const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";

    std::string path = "./tmpfile.txt";
    std::ofstream file(path, std::fstream::trunc);
    std::vector<std::string> lines;
    std::string line;

    for (int l = 0; l < 10000; l++) {
        for (int i = 0; i < rand() % 100; ++i) {
            line += alphanum[rand() % (sizeof(alphanum) - 1)];
        }
        file << line << std::endl;
        lines.push_back(std::move(line));
    }

    io->textFile2(path, n);

    CPPUNIT_ASSERT_GREATEREQUAL(n, (int)executor_data->getPartitions<std::string>()->partitions());

    auto result = getFromPartitions<std::string>();

    loadToPartitions(result, 1);

    executor_data->mpi().gather(*((*executor_data->getPartitions<std::string>())[0]), 0);

    result = getFromPartitions<std::string>();

    if(executor_data->mpi().isRoot(0)){
        CPPUNIT_ASSERT_EQUAL(lines.size(), result.size());
        for (int i = 0; i < result.size(); i++) {
            CPPUNIT_ASSERT_EQUAL(lines[i], result[i]);
        }
    }

}