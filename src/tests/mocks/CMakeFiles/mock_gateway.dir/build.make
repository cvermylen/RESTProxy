# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.11

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.11.0/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.11.0/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/christian/projects/RESTProxy

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/christian/projects/RESTProxy/src

# Include any dependencies generated for this target.
include tests/mocks/CMakeFiles/mock_gateway.dir/depend.make

# Include the progress variables for this target.
include tests/mocks/CMakeFiles/mock_gateway.dir/progress.make

# Include the compile flags for this target's objects.
include tests/mocks/CMakeFiles/mock_gateway.dir/flags.make

tests/mocks/CMakeFiles/mock_gateway.dir/mock_socket.c.o: tests/mocks/CMakeFiles/mock_gateway.dir/flags.make
tests/mocks/CMakeFiles/mock_gateway.dir/mock_socket.c.o: ../tests/mocks/mock_socket.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/christian/projects/RESTProxy/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object tests/mocks/CMakeFiles/mock_gateway.dir/mock_socket.c.o"
	cd /Users/christian/projects/RESTProxy/src/tests/mocks && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mock_gateway.dir/mock_socket.c.o   -c /Users/christian/projects/RESTProxy/tests/mocks/mock_socket.c

tests/mocks/CMakeFiles/mock_gateway.dir/mock_socket.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mock_gateway.dir/mock_socket.c.i"
	cd /Users/christian/projects/RESTProxy/src/tests/mocks && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/christian/projects/RESTProxy/tests/mocks/mock_socket.c > CMakeFiles/mock_gateway.dir/mock_socket.c.i

tests/mocks/CMakeFiles/mock_gateway.dir/mock_socket.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mock_gateway.dir/mock_socket.c.s"
	cd /Users/christian/projects/RESTProxy/src/tests/mocks && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/christian/projects/RESTProxy/tests/mocks/mock_socket.c -o CMakeFiles/mock_gateway.dir/mock_socket.c.s

tests/mocks/CMakeFiles/mock_gateway.dir/mock_log.c.o: tests/mocks/CMakeFiles/mock_gateway.dir/flags.make
tests/mocks/CMakeFiles/mock_gateway.dir/mock_log.c.o: ../tests/mocks/mock_log.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/christian/projects/RESTProxy/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object tests/mocks/CMakeFiles/mock_gateway.dir/mock_log.c.o"
	cd /Users/christian/projects/RESTProxy/src/tests/mocks && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mock_gateway.dir/mock_log.c.o   -c /Users/christian/projects/RESTProxy/tests/mocks/mock_log.c

tests/mocks/CMakeFiles/mock_gateway.dir/mock_log.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mock_gateway.dir/mock_log.c.i"
	cd /Users/christian/projects/RESTProxy/src/tests/mocks && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/christian/projects/RESTProxy/tests/mocks/mock_log.c > CMakeFiles/mock_gateway.dir/mock_log.c.i

tests/mocks/CMakeFiles/mock_gateway.dir/mock_log.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mock_gateway.dir/mock_log.c.s"
	cd /Users/christian/projects/RESTProxy/src/tests/mocks && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/christian/projects/RESTProxy/tests/mocks/mock_log.c -o CMakeFiles/mock_gateway.dir/mock_log.c.s

# Object files for target mock_gateway
mock_gateway_OBJECTS = \
"CMakeFiles/mock_gateway.dir/mock_socket.c.o" \
"CMakeFiles/mock_gateway.dir/mock_log.c.o"

# External object files for target mock_gateway
mock_gateway_EXTERNAL_OBJECTS =

tests/mocks/libmock_gateway.a: tests/mocks/CMakeFiles/mock_gateway.dir/mock_socket.c.o
tests/mocks/libmock_gateway.a: tests/mocks/CMakeFiles/mock_gateway.dir/mock_log.c.o
tests/mocks/libmock_gateway.a: tests/mocks/CMakeFiles/mock_gateway.dir/build.make
tests/mocks/libmock_gateway.a: tests/mocks/CMakeFiles/mock_gateway.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/christian/projects/RESTProxy/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C static library libmock_gateway.a"
	cd /Users/christian/projects/RESTProxy/src/tests/mocks && $(CMAKE_COMMAND) -P CMakeFiles/mock_gateway.dir/cmake_clean_target.cmake
	cd /Users/christian/projects/RESTProxy/src/tests/mocks && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mock_gateway.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/mocks/CMakeFiles/mock_gateway.dir/build: tests/mocks/libmock_gateway.a

.PHONY : tests/mocks/CMakeFiles/mock_gateway.dir/build

tests/mocks/CMakeFiles/mock_gateway.dir/clean:
	cd /Users/christian/projects/RESTProxy/src/tests/mocks && $(CMAKE_COMMAND) -P CMakeFiles/mock_gateway.dir/cmake_clean.cmake
.PHONY : tests/mocks/CMakeFiles/mock_gateway.dir/clean

tests/mocks/CMakeFiles/mock_gateway.dir/depend:
	cd /Users/christian/projects/RESTProxy/src && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/christian/projects/RESTProxy /Users/christian/projects/RESTProxy/tests/mocks /Users/christian/projects/RESTProxy/src /Users/christian/projects/RESTProxy/src/tests/mocks /Users/christian/projects/RESTProxy/src/tests/mocks/CMakeFiles/mock_gateway.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/mocks/CMakeFiles/mock_gateway.dir/depend
