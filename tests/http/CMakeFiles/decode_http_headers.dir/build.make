# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Produce verbose output by default.
VERBOSE = 1

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/christian/CLionProjects/RESTProxy

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/christian/CLionProjects/RESTProxy

# Include any dependencies generated for this target.
include tests/http/CMakeFiles/decode_http_headers.dir/depend.make

# Include the progress variables for this target.
include tests/http/CMakeFiles/decode_http_headers.dir/progress.make

# Include the compile flags for this target's objects.
include tests/http/CMakeFiles/decode_http_headers.dir/flags.make

tests/http/CMakeFiles/decode_http_headers.dir/decode_http_headers.c.o: tests/http/CMakeFiles/decode_http_headers.dir/flags.make
tests/http/CMakeFiles/decode_http_headers.dir/decode_http_headers.c.o: tests/http/decode_http_headers.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/christian/CLionProjects/RESTProxy/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object tests/http/CMakeFiles/decode_http_headers.dir/decode_http_headers.c.o"
	cd /home/christian/CLionProjects/RESTProxy/tests/http && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/decode_http_headers.dir/decode_http_headers.c.o   -c /home/christian/CLionProjects/RESTProxy/tests/http/decode_http_headers.c

tests/http/CMakeFiles/decode_http_headers.dir/decode_http_headers.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/decode_http_headers.dir/decode_http_headers.c.i"
	cd /home/christian/CLionProjects/RESTProxy/tests/http && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/christian/CLionProjects/RESTProxy/tests/http/decode_http_headers.c > CMakeFiles/decode_http_headers.dir/decode_http_headers.c.i

tests/http/CMakeFiles/decode_http_headers.dir/decode_http_headers.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/decode_http_headers.dir/decode_http_headers.c.s"
	cd /home/christian/CLionProjects/RESTProxy/tests/http && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/christian/CLionProjects/RESTProxy/tests/http/decode_http_headers.c -o CMakeFiles/decode_http_headers.dir/decode_http_headers.c.s

tests/http/CMakeFiles/decode_http_headers.dir/decode_http_headers.c.o.requires:

.PHONY : tests/http/CMakeFiles/decode_http_headers.dir/decode_http_headers.c.o.requires

tests/http/CMakeFiles/decode_http_headers.dir/decode_http_headers.c.o.provides: tests/http/CMakeFiles/decode_http_headers.dir/decode_http_headers.c.o.requires
	$(MAKE) -f tests/http/CMakeFiles/decode_http_headers.dir/build.make tests/http/CMakeFiles/decode_http_headers.dir/decode_http_headers.c.o.provides.build
.PHONY : tests/http/CMakeFiles/decode_http_headers.dir/decode_http_headers.c.o.provides

tests/http/CMakeFiles/decode_http_headers.dir/decode_http_headers.c.o.provides.build: tests/http/CMakeFiles/decode_http_headers.dir/decode_http_headers.c.o


# Object files for target decode_http_headers
decode_http_headers_OBJECTS = \
"CMakeFiles/decode_http_headers.dir/decode_http_headers.c.o"

# External object files for target decode_http_headers
decode_http_headers_EXTERNAL_OBJECTS = \
"/home/christian/CLionProjects/RESTProxy/src/http/CMakeFiles/http_headers.o.dir/http_headers.c.o" \
"/home/christian/CLionProjects/RESTProxy/tests/mocks/CMakeFiles/mock_socket_connector.o.dir/mock_socket_connector.c.o" \
"/home/christian/CLionProjects/RESTProxy/tests/mocks/CMakeFiles/mock_shared_buffers.o.dir/mock_shared_buffers.c.o" \
"/home/christian/CLionProjects/RESTProxy/src/buffers/CMakeFiles/circular_buffer.o.dir/circular_buffer.c.o"

bin/decode_http_headers: tests/http/CMakeFiles/decode_http_headers.dir/decode_http_headers.c.o
bin/decode_http_headers: src/http/CMakeFiles/http_headers.o.dir/http_headers.c.o
bin/decode_http_headers: tests/mocks/CMakeFiles/mock_socket_connector.o.dir/mock_socket_connector.c.o
bin/decode_http_headers: tests/mocks/CMakeFiles/mock_shared_buffers.o.dir/mock_shared_buffers.c.o
bin/decode_http_headers: src/buffers/CMakeFiles/circular_buffer.o.dir/circular_buffer.c.o
bin/decode_http_headers: tests/http/CMakeFiles/decode_http_headers.dir/build.make
bin/decode_http_headers: tests/http/CMakeFiles/decode_http_headers.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/christian/CLionProjects/RESTProxy/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable ../../bin/decode_http_headers"
	cd /home/christian/CLionProjects/RESTProxy/tests/http && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/decode_http_headers.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/http/CMakeFiles/decode_http_headers.dir/build: bin/decode_http_headers

.PHONY : tests/http/CMakeFiles/decode_http_headers.dir/build

tests/http/CMakeFiles/decode_http_headers.dir/requires: tests/http/CMakeFiles/decode_http_headers.dir/decode_http_headers.c.o.requires

.PHONY : tests/http/CMakeFiles/decode_http_headers.dir/requires

tests/http/CMakeFiles/decode_http_headers.dir/clean:
	cd /home/christian/CLionProjects/RESTProxy/tests/http && $(CMAKE_COMMAND) -P CMakeFiles/decode_http_headers.dir/cmake_clean.cmake
.PHONY : tests/http/CMakeFiles/decode_http_headers.dir/clean

tests/http/CMakeFiles/decode_http_headers.dir/depend:
	cd /home/christian/CLionProjects/RESTProxy && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/christian/CLionProjects/RESTProxy /home/christian/CLionProjects/RESTProxy/tests/http /home/christian/CLionProjects/RESTProxy /home/christian/CLionProjects/RESTProxy/tests/http /home/christian/CLionProjects/RESTProxy/tests/http/CMakeFiles/decode_http_headers.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/http/CMakeFiles/decode_http_headers.dir/depend

