
#include "IShuffleModuleTest.h"

using namespace ignis::executor::core::modules;

void IShuffleModuleTest::setUp() {
    shuffle_module = std::make_shared<IShuffleModule>(executor_data);
}

void IShuffleModuleTest::tearDown() {

}

void IShuffleModuleTest::shuffle() {

}


