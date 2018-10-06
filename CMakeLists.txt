cmake_minimum_required(VERSION 3.6)
project(ignis)

ADD_SUBDIRECTORY(ignis)

option(BUILD_TESTING "Enable testing" OFF)
#if(BUILD_TESTING)
#    enable_testing()
    ADD_SUBDIRECTORY(ignis-test)
#endif()

