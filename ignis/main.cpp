#include <cstdlib>
#include <iostream>
#include <ctime>
#include "executor/core/ILog.h"
#include "data/RTTInfo.h"

using namespace ignis::executor::core;

int main(int argc, char *argv[]) {



    IGNIS_LOG_INIT();
    IGNIS_LOG(trace) << "TEST";

    IGNIS_LOG(trace) << "A trace severity message";
    IGNIS_LOG(debug) << "A debug severity message";
    IGNIS_LOG(info) << "An informational severity message";
    IGNIS_LOG(warning) << "A warning severity message";
    IGNIS_LOG(error) << "An error severity message";
    IGNIS_LOG(fatal) << "A fatal severity message";

    std::cout << ignis::data::RTTInfo(typeid("hola")).getStandardName() << std::endl;

    return EXIT_SUCCESS;
}
