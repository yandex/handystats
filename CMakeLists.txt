CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

PROJECT (handystats)

ADD_DEFINITIONS ("-O2 -std=c++0x -Wreorder -Wreturn-type -Wunused-variable -pedantic")

INCLUDE_DIRECTORIES (${PROJECT_SOURCE_DIR}/include)

FILE (GLOB_RECURSE metrics_src src/metrics/ "*.cpp")
ADD_LIBRARY (metrics SHARED ${metrics_src})

ENABLE_TESTING ()
ADD_SUBDIRECTORY (tests)

