cmake_minimum_required(VERSION 3.6)
project(ignis)

option(BUILD_TESTING "Enable testing" OFF)
option(BUILD_COVERAGE "Enable coverage report" OFF)

if(BUILD_COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fPIC -fno-inline -fno-default-inline -fno-inline-small-functions")
endif()

ADD_SUBDIRECTORY(src)

if(BUILD_TESTING)
    ADD_SUBDIRECTORY(test)
endif()

