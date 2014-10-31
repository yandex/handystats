CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

PROJECT (HANDYSTATS_COMBAINE_PLUGIN)

ADD_CUSTOM_TARGET (handystats_aggregator ALL
	COMMAND ${CMAKE_COMMAND} -E copy
		${CMAKE_CURRENT_SOURCE_DIR}/handystats_aggregator.py ${CMAKE_CURRENT_BINARY_DIR}/handystats_aggregator.py
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/handystats_aggregator.py
	)

INSTALL (PROGRAMS handystats_aggregator.py
	DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/yandex/combaine/custom
	)
