CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

PROJECT (handystats)

ADD_DEFINITIONS ("-O2 -std=c++0x -Wreorder -Wreturn-type -Wunused-variable -pedantic")

INCLUDE_DIRECTORIES (${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src)

FILE (STRINGS ${PROJECT_SOURCE_DIR}/source_files.cmake handy_src)
ADD_LIBRARY (handy STATIC ${handy_src})

ADD_CUSTOM_TARGET (check COMMAND ${CMAKE_CTEST_COMMAND})

ENABLE_TESTING ()
ADD_SUBDIRECTORY (tests)

