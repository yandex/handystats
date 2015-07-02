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

SET (BENCHMARK_LIBRARIES ${PROJECT_NAME})

FIND_PACKAGE (Threads)
IF (Threads_FOUND)
	INCLUDE_DIRECTORIES (${Threads_INCLUDE_DIR})
	LIST (APPEND BENCHMARK_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})
ENDIF ()

FIND_PACKAGE (Boost COMPONENTS program_options)
IF (Boost_PROGRAM_OPTIONS_FOUND)
	INCLUDE_DIRECTORIES (${Boost_INCLUDE_DIR})
	LIST (APPEND BENCHMARK_LIBRARIES ${Boost_PROGRAM_OPTIONS_LIBRARY})
ENDIF ()

SET (BENCHMARK_LINK_FLAGS "-Wl,-rpath,${CMAKE_CURRENT_BINARY_DIR}:${CMAKE_CURRENT_BINARY_DIR}/../${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
SET (BENCHMARK_PROPERTIES PROPERTIES LINK_FLAGS "${BENCHMARK_LINK_FLAGS}" LINKER_LANGUAGE CXX)

ADD_CUSTOM_TARGET(benchmarks)

ADD_EXECUTABLE (load EXCLUDE_FROM_ALL ${CMAKE_CURRENT_SOURCE_DIR}/load.cpp)
SET_TARGET_PROPERTIES (load ${BENCHMARK_PROPERTIES})
TARGET_LINK_LIBRARIES (load ${BENCHMARK_LIBRARIES})
ADD_DEPENDENCIES (benchmarks load)

FILE (COPY run_load.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
