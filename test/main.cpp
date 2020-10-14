
#include "ignis/executor/core/ILog.h"
#include <boost/filesystem.hpp>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <fcntl.h>
#include <mpi.h>
#include <sys/stat.h>

#include "ignis/executor/core/IMpiTest.h"
#include "ignis/executor/core/storage/IDiskPartitionTest.h"
#include "ignis/executor/core/storage/IMemoryPartitionTest.h"
#include "ignis/executor/core/storage/IRawMemoryPartitionTest.h"

#include "ignis/executor/core/modules/IGeneralActionModuleTest.h"
#include "ignis/executor/core/modules/IGeneralModuleTest.h"
#include "ignis/executor/core/modules/IIOModuleTest.h"


#define MPI_TEST "mpi_test"
#define PARTITION_TEST "partition_test"
#define MODULE_TEST "module_test"

using namespace ignis::executor::core;

typedef std::pair<int, std::string> PairIntString;
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(storage::IMemoryPartitionTest<bool>, PARTITION_TEST);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(storage::IMemoryPartitionTest<int>, PARTITION_TEST);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(storage::IRawMemoryPartitionTest<int>, PARTITION_TEST);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(storage::IRawMemoryPartitionTest<std::string>, PARTITION_TEST);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(storage::IRawMemoryPartitionTest<uint8_t>, PARTITION_TEST);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(storage::IRawMemoryPartitionTest<PairIntString>, PARTITION_TEST);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(storage::IDiskPartitionTest<int>, PARTITION_TEST);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(storage::IDiskPartitionTest<std::string>, PARTITION_TEST);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(storage::IDiskPartitionTest<uint8_t>, PARTITION_TEST);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(storage::IDiskPartitionTest<PairIntString>, PARTITION_TEST);

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(IMpiTest<storage::IMemoryPartition<int>>, MPI_TEST);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(IMpiTest<storage::IMemoryPartition<std::string>>, MPI_TEST);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(IMpiTest<storage::IRawMemoryPartition<int>>, MPI_TEST);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(IMpiTest<storage::IDiskPartition<int>>, MPI_TEST);

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(modules::IGeneralModuleTest, MODULE_TEST);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(modules::IGeneralActionModuleTest, MODULE_TEST);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(modules::IIOModuleTest, MODULE_TEST);


int main(int argc, char *argv[]) {
    MPI::Init(argc, argv);
    int rank = MPI::COMM_WORLD.Get_rank();
    bool parallel = MPI::COMM_WORLD.Get_size() > 1;
    std::ofstream logFile;
    std::streambuf *bak[2] = {std::cerr.rdbuf(), std::cout.rdbuf()};
    if (parallel) {
        std::string wd = "np" + std::to_string(rank);
        boost::filesystem::create_directory(wd);
        ::chdir(wd.c_str());
        if (rank > 0) {
            logFile.open("test.log", std::ofstream::trunc);
            std::cerr.rdbuf(logFile.rdbuf());
            std::cout.rdbuf(logFile.rdbuf());
        }
    }
    IGNIS_LOG_INIT();
    IGNIS_LOG_ENABLE(false);
    CPPUNIT_NS::TestResult results;
    CPPUNIT_NS::TestResultCollector result_collector;
    CPPUNIT_NS::BriefTestProgressListener progress;
    CPPUNIT_NS::TestRunner runner;
    CPPUNIT_NS::CompilerOutputter compileroutputter(&result_collector, std::cerr);

    results.addListener(&result_collector);
    results.addListener(&progress);
    runner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry(PARTITION_TEST).makeTest());
    if (parallel) { runner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry(MPI_TEST).makeTest()); }
    runner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry(MODULE_TEST).makeTest());
    runner.run(results);
    compileroutputter.write();
    MPI::Finalize();

    if (!parallel) { std::cerr << "WARNING: mpi test skipped" << std::endl; }
    std::cerr.rdbuf(bak[0]);
    std::cout.rdbuf(bak[1]);

    return result_collector.wasSuccessful() ? 0 : 1;
}
