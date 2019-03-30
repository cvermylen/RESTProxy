cmake_minimum_required (VERSION 3.1.0)

project(Gateway)

# Add ./cmake to the module path
list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option (ENABLE_TESTS "Build tests along with the library" ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_subdirectory(src)

if (ENABLE_TESTS)
	# Check for Criterion
	find_package (Criterion REQUIRED)
	find_program(MEMORYCHECK_COMMAND NAMES valgrind)
	set(MEMORYCHECK_COMMAND_OPTIONS "--xml=yes --xml-file=test.xml")
	file(TO_CMAKE_PATH "$(CMAKE_SOURCE_DIR}/resources/valgrind_supp.xml" MEMORYCHECK_SUPPRESSION_FILE)

	# Add tests
	include(CTest)
	add_subdirectory(tests)
endif()
