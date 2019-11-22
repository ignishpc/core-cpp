cmake_minimum_required(VERSION 3.6)
project(ignis)

ADD_SUBDIRECTORY(src)

option(BUILD_TESTING "Enable testing" ON)
if(BUILD_TESTING)
    ADD_SUBDIRECTORY(test)
endif()

