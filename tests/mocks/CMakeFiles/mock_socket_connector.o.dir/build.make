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
include tests/mocks/CMakeFiles/mock_socket_connector.o.dir/depend.make

# Include the progress variables for this target.
include tests/mocks/CMakeFiles/mock_socket_connector.o.dir/progress.make

# Include the compile flags for this target's objects.
include tests/mocks/CMakeFiles/mock_socket_connector.o.dir/flags.make

tests/mocks/CMakeFiles/mock_socket_connector.o.dir/mock_socket_connector.c.o: tests/mocks/CMakeFiles/mock_socket_connector.o.dir/flags.make
tests/mocks/CMakeFiles/mock_socket_connector.o.dir/mock_socket_connector.c.o: tests/mocks/mock_socket_connector.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/christian/CLionProjects/RESTProxy/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object tests/mocks/CMakeFiles/mock_socket_connector.o.dir/mock_socket_connector.c.o"
	cd /home/christian/CLionProjects/RESTProxy/tests/mocks && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mock_socket_connector.o.dir/mock_socket_connector.c.o   -c /home/christian/CLionProjects/RESTProxy/tests/mocks/mock_socket_connector.c

tests/mocks/CMakeFiles/mock_socket_connector.o.dir/mock_socket_connector.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mock_socket_connector.o.dir/mock_socket_connector.c.i"
	cd /home/christian/CLionProjects/RESTProxy/tests/mocks && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/christian/CLionProjects/RESTProxy/tests/mocks/mock_socket_connector.c > CMakeFiles/mock_socket_connector.o.dir/mock_socket_connector.c.i

tests/mocks/CMakeFiles/mock_socket_connector.o.dir/mock_socket_connector.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mock_socket_connector.o.dir/mock_socket_connector.c.s"
	cd /home/christian/CLionProjects/RESTProxy/tests/mocks && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/christian/CLionProjects/RESTProxy/tests/mocks/mock_socket_connector.c -o CMakeFiles/mock_socket_connector.o.dir/mock_socket_connector.c.s

tests/mocks/CMakeFiles/mock_socket_connector.o.dir/mock_socket_connector.c.o.requires:

.PHONY : tests/mocks/CMakeFiles/mock_socket_connector.o.dir/mock_socket_connector.c.o.requires

tests/mocks/CMakeFiles/mock_socket_connector.o.dir/mock_socket_connector.c.o.provides: tests/mocks/CMakeFiles/mock_socket_connector.o.dir/mock_socket_connector.c.o.requires
	$(MAKE) -f tests/mocks/CMakeFiles/mock_socket_connector.o.dir/build.make tests/mocks/CMakeFiles/mock_socket_connector.o.dir/mock_socket_connector.c.o.provides.build
.PHONY : tests/mocks/CMakeFiles/mock_socket_connector.o.dir/mock_socket_connector.c.o.provides

tests/mocks/CMakeFiles/mock_socket_connector.o.dir/mock_socket_connector.c.o.provides.build: tests/mocks/CMakeFiles/mock_socket_connector.o.dir/mock_socket_connector.c.o


mock_socket_connector.o: tests/mocks/CMakeFiles/mock_socket_connector.o.dir/mock_socket_connector.c.o
mock_socket_connector.o: tests/mocks/CMakeFiles/mock_socket_connector.o.dir/build.make

.PHONY : mock_socket_connector.o

# Rule to build all files generated by this target.
tests/mocks/CMakeFiles/mock_socket_connector.o.dir/build: mock_socket_connector.o

.PHONY : tests/mocks/CMakeFiles/mock_socket_connector.o.dir/build

tests/mocks/CMakeFiles/mock_socket_connector.o.dir/requires: tests/mocks/CMakeFiles/mock_socket_connector.o.dir/mock_socket_connector.c.o.requires

.PHONY : tests/mocks/CMakeFiles/mock_socket_connector.o.dir/requires

tests/mocks/CMakeFiles/mock_socket_connector.o.dir/clean:
	cd /home/christian/CLionProjects/RESTProxy/tests/mocks && $(CMAKE_COMMAND) -P CMakeFiles/mock_socket_connector.o.dir/cmake_clean.cmake
.PHONY : tests/mocks/CMakeFiles/mock_socket_connector.o.dir/clean

tests/mocks/CMakeFiles/mock_socket_connector.o.dir/depend:
	cd /home/christian/CLionProjects/RESTProxy && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/christian/CLionProjects/RESTProxy /home/christian/CLionProjects/RESTProxy/tests/mocks /home/christian/CLionProjects/RESTProxy /home/christian/CLionProjects/RESTProxy/tests/mocks /home/christian/CLionProjects/RESTProxy/tests/mocks/CMakeFiles/mock_socket_connector.o.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/mocks/CMakeFiles/mock_socket_connector.o.dir/depend

