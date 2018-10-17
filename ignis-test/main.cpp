
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestResult.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCase.h>
#include "../ignis/executor/core/ILog.h"

#include "executor/core/storage/IMemoryObjectTest.h"
#include "executor/core/storage/IRawMemoryObjectTest.h"

#include "executor/core/modules/IMapperModuleTest.h"
#include "executor/core/modules/IShuffleModuleTest.h"
#include "executor/core/modules/IPostmanModuleTest.h"


CPPUNIT_TEST_SUITE_REGISTRATION(ignis::executor::core::storage::IMemoryObjectTest);
CPPUNIT_TEST_SUITE_REGISTRATION(ignis::executor::core::storage::IRawMemoryObjectTest);

CPPUNIT_TEST_SUITE_REGISTRATION(ignis::executor::core::modules::IMapperModuleTest);
//CPPUNIT_TEST_SUITE_REGISTRATION(ignis::executor::core::modules::IShuffleModuleTest);
//CPPUNIT_TEST_SUITE_REGISTRATION(ignis::executor::core::modules::IPostmanModuleTest);

int main(int argc, char *argv[]) {
    IGNIS_LOG_INIT();
    IGNIS_LOG_ENABLE(false);
    CPPUNIT_NS::TestResult results;
    CPPUNIT_NS::TestResultCollector result_collector;
    CPPUNIT_NS::BriefTestProgressListener progress;
    CPPUNIT_NS::TestRunner runner;
    CPPUNIT_NS::CompilerOutputter compileroutputter(&result_collector, std::cerr);

    results.addListener(&result_collector);
    results.addListener(&progress);
    runner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
    runner.run(results);
    compileroutputter.write();

    return result_collector.wasSuccessful() ? 0 : 1;
}













