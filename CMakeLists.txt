CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

PROJECT (handystats)

ADD_DEFINITIONS ("-O2 -std=c++0x -Wreorder -Wreturn-type -Wunused-variable -pedantic")

INCLUDE_DIRECTORIES (${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src)

FILE (GLOB_RECURSE handy_src src/ "*.cpp")
ADD_LIBRARY (handy STATIC ${handy_src})

ENABLE_TESTING ()
ADD_SUBDIRECTORY (tests)

