CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

PROJECT (HANDYSTATS_PYTHON)

FIND_PACKAGE (PythonInterp 2.6 REQUIRED)
FIND_PACKAGE (PythonLibs 2.6 REQUIRED)
EXECUTE_PROCESS (COMMAND
	${PYTHON_EXECUTABLE} -c "from distutils.sysconfig import get_python_lib; print (get_python_lib())"
	OUTPUT_VARIABLE PYTHON_SITE_PACKAGES
	OUTPUT_STRIP_TRAILING_WHITESPACE)

MESSAGE (STATUS "Python includes are situated in ${PYTHON_INCLUDE_DIRS}")
MESSAGE (STATUS "Python site-packages are situated in ${PYTHON_SITE_PACKAGES}")

ADD_CUSTOM_TARGET (bindings_python
	COMMAND ${PYTHON_EXECUTABLE} setup.py build_py
		--build-lib ${CMAKE_CURRENT_BINARY_DIR}
		--force
		--verbose
	COMMAND ${PYTHON_EXECUTABLE} setup.py build_ext
		--build-lib ${CMAKE_CURRENT_BINARY_DIR}
		--include-dirs "${HANDYSTATS_INCLUDE_DIR}"
		--library-dirs "${HANDYSTATS_LIBRARY_OUTPUT_DIR}"
		--force
		--cython-c-in-temp
		--verbose
	COMMAND ${PYTHON_EXECUTABLE} setup.py clean
		--build-lib ${CMAKE_CURRENT_BINARY_DIR}
		--verbose
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)

ADD_DEPENDENCIES (bindings_python ${HANDYSTATS_LIBRARY})
