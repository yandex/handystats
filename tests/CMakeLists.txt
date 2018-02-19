#
# Copyright (c) YANDEX LLC. All rights reserved.
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 3.0 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library.
#

CMAKE_MINIMUM_REQUIRED (VERSION 2.8)


FIND_PACKAGE (Threads REQUIRED)
INCLUDE_DIRECTORIES (${Threads_INCLUDE_DIR})

ADD_SUBDIRECTORY (${PROJECT_SOURCE_DIR}/thirdparty/googletest/googletest ${CMAKE_BINARY_DIR}/googletest)
INCLUDE_DIRECTORIES (${gtest_SOURCE_DIR}/include)

SET (GTEST_TARGETS gtest gtest_main)


ADD_CUSTOM_TARGET (check COMMAND ${CMAKE_CTEST_COMMAND} -V)


SET (TEST_LINK_FLAGS "-Wl,-rpath,${CMAKE_CURRENT_BINARY_DIR}:${CMAKE_CURRENT_BINARY_DIR}/../${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
SET (TEST_PROPERTIES PROPERTIES LINK_FLAGS "${TEST_LINK_FLAGS}" LINKER_LANGUAGE CXX)
SET (TEST_LIBRARIES gtest_main ${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})

FILE (GLOB_RECURSE gtest_src_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.gtest.cpp")

FOREACH (gtest_src_file IN LISTS gtest_src_files)
	STRING (REGEX REPLACE "[.]gtest[.]cpp$" "_gtest" gtest_exec_file ${gtest_src_file})
	ADD_EXECUTABLE (${gtest_exec_file} EXCLUDE_FROM_ALL ${CMAKE_CURRENT_SOURCE_DIR}/${gtest_src_file})
	SET_TARGET_PROPERTIES (${gtest_exec_file} ${TEST_PROPERTIES})
	TARGET_LINK_LIBRARIES (${gtest_exec_file} ${TEST_LIBRARIES})
	ADD_TEST (${gtest_exec_file} ${gtest_exec_file})
	ADD_DEPENDENCIES (${gtest_exec_file} ${GTEST_TARGETS})
	ADD_DEPENDENCIES (check ${gtest_exec_file})
ENDFOREACH (gtest_src_file)

# C-Binging test
SET (CBINDING_TEST_LINK_FLAGS "-Wl,-rpath,${CMAKE_CURRENT_BINARY_DIR}:${CMAKE_CURRENT_BINARY_DIR}/../${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
SET (CBINDING_TEST_PROPERTIES_CXX PROPERTIES LINK_FLAGS "${CBINDING_TEST_LINK_FLAGS}" LINKER_LANGUAGE CXX)
SET (CBINDING_TEST_PROPERTIES_C PROPERTIES LINK_FLAGS "${CBINDING_TEST_LINK_FLAGS}" LINKER_LANGUAGE C)
SET (CBINDING_TEST_LIBRARIES gtest ${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})

ADD_LIBRARY (cbinding_test STATIC EXCLUDE_FROM_ALL ${CMAKE_CURRENT_SOURCE_DIR}/c-binding/test.c)
SET_TARGET_PROPERTIES (cbinding_test ${CBINDING_TEST_PROPERTIES_C})
TARGET_LINK_LIBRARIES (cbinding_test ${CBINDING_TEST_LIBRARIES})

ADD_EXECUTABLE (cbinding_check EXCLUDE_FROM_ALL ${CMAKE_CURRENT_SOURCE_DIR}/c-binding/check.cpp)
SET_TARGET_PROPERTIES (cbinding_check ${CBINDING_TEST_PROPERTIES_CXX})
TARGET_LINK_LIBRARIES (cbinding_check ${CBINDING_TEST_LIBRARIES} cbinding_test)

ADD_TEST(cbinding_check cbinding_check)
ADD_DEPENDENCIES (cbinding_check ${GTEST_TARGETS} cbinding_test)
ADD_DEPENDENCIES (check cbinding_check)
