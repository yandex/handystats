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

# Generate bindings sources with cython
FIND_PROGRAM (CYTHON_EXECUTABLE
	NAMES cython cython2
	PATHS /usr/bin)
IF (CYTHON_EXECUTABLE-NOTFOUND)
	MESSAGE (FATAL_ERROR "Cython executable not found.")
ELSE ()
	MESSAGE (STATUS "Cython executable found: ${CYTHON_EXECUTABLE}")
ENDIF ()

ADD_CUSTOM_TARGET (generate_bindings_python ALL)

FILE (GLOB_RECURSE CYTHON_SRC_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "handystats/*.pyx")

FOREACH (CYTHON_SRC_FILE IN LISTS CYTHON_SRC_FILES)
	STRING (REGEX REPLACE "^handystats/(.+)[.]pyx$" "\\1" CYTHON_MODULE_NAME ${CYTHON_SRC_FILE})
	MESSAGE (STATUS "Cython module: ${MODULE_NAME}")
	ADD_CUSTOM_TARGET (generate_bindings_python_module_${CYTHON_MODULE_NAME}
		COMMAND ${CYTHON_EXECUTABLE} ${CYTHON_SRC_FILE}
			--include-dir "${HANDYSTATS_INCLUDE_DIR}"
			--cplus
			--output-file "handystats/${CYTHON_MODULE_NAME}.cpp"
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		)
	ADD_DEPENDENCIES (generate_bindings_python generate_bindings_python_module_${CYTHON_MODULE_NAME})
ENDFOREACH ()

ADD_CUSTOM_TARGET (bindings_python
	COMMAND ${PYTHON_EXECUTABLE} setup.py build_py
	--build-lib ${CMAKE_CURRENT_BINARY_DIR}
	--verbose
	COMMAND ${PYTHON_EXECUTABLE} setup.py build_ext
	--build-lib ${CMAKE_CURRENT_BINARY_DIR}
	--include-dirs "${HANDYSTATS_INCLUDE_DIR}"
	--library-dirs "${HANDYSTATS_LIBRARY_OUTPUT_DIR}"
	--verbose
	COMMAND ${PYTHON_EXECUTABLE} setup.py clean
	--build-lib ${CMAKE_CURRENT_BINARY_DIR}
	--verbose
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)

ADD_DEPENDENCIES (bindings_python generate_bindings_python ${HANDYSTATS_LIBRARY})
