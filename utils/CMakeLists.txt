CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

PROJECT (HANDYSTATS_UTILS)

ADD_CUSTOM_TARGET(utils)

# aggregate_log

ADD_CUSTOM_TARGET (aggregate_log
	COMMAND ${CMAKE_COMMAND} -E copy
		${CMAKE_CURRENT_SOURCE_DIR}/aggregate_log ${CMAKE_CURRENT_BINARY_DIR}/aggregate_log
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/aggregate_log
	)

INSTALL (PROGRAMS aggregate_log
	DESTINATION bin
	COMPONENT utilities
	)

ADD_DEPENDENCIES (utils aggregate_log)

ADD_CUSTOM_TARGET (install_utilities
	${CMAKE_COMMAND}
	-D "CMAKE_INSTALL_COMPONENT=utilities"
	-P "${CMAKE_CURRENT_BINARY_DIR}/cmake_install.cmake"
	)
ADD_DEPENDENCIES (install_utilities utils)
