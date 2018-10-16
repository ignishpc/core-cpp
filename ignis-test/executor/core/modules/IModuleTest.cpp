
#include "IModuleTest.h"

using namespace ignis::executor::core::modules;

IModuleTest::IModuleTest() : executor_data(std::make_shared<IExecutorData>()),library("./libtestfunctions.so") {}



