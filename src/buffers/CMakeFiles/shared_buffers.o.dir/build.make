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
include src/buffers/CMakeFiles/shared_buffers.o.dir/depend.make

# Include the progress variables for this target.
include src/buffers/CMakeFiles/shared_buffers.o.dir/progress.make

# Include the compile flags for this target's objects.
include src/buffers/CMakeFiles/shared_buffers.o.dir/flags.make

src/buffers/CMakeFiles/shared_buffers.o.dir/shared_buffers.c.o: src/buffers/CMakeFiles/shared_buffers.o.dir/flags.make
src/buffers/CMakeFiles/shared_buffers.o.dir/shared_buffers.c.o: src/buffers/shared_buffers.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/christian/CLionProjects/RESTProxy/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/buffers/CMakeFiles/shared_buffers.o.dir/shared_buffers.c.o"
	cd /home/christian/CLionProjects/RESTProxy/src/buffers && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/shared_buffers.o.dir/shared_buffers.c.o   -c /home/christian/CLionProjects/RESTProxy/src/buffers/shared_buffers.c

src/buffers/CMakeFiles/shared_buffers.o.dir/shared_buffers.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/shared_buffers.o.dir/shared_buffers.c.i"
	cd /home/christian/CLionProjects/RESTProxy/src/buffers && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/christian/CLionProjects/RESTProxy/src/buffers/shared_buffers.c > CMakeFiles/shared_buffers.o.dir/shared_buffers.c.i

src/buffers/CMakeFiles/shared_buffers.o.dir/shared_buffers.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/shared_buffers.o.dir/shared_buffers.c.s"
	cd /home/christian/CLionProjects/RESTProxy/src/buffers && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/christian/CLionProjects/RESTProxy/src/buffers/shared_buffers.c -o CMakeFiles/shared_buffers.o.dir/shared_buffers.c.s

src/buffers/CMakeFiles/shared_buffers.o.dir/shared_buffers.c.o.requires:

.PHONY : src/buffers/CMakeFiles/shared_buffers.o.dir/shared_buffers.c.o.requires

src/buffers/CMakeFiles/shared_buffers.o.dir/shared_buffers.c.o.provides: src/buffers/CMakeFiles/shared_buffers.o.dir/shared_buffers.c.o.requires
	$(MAKE) -f src/buffers/CMakeFiles/shared_buffers.o.dir/build.make src/buffers/CMakeFiles/shared_buffers.o.dir/shared_buffers.c.o.provides.build
.PHONY : src/buffers/CMakeFiles/shared_buffers.o.dir/shared_buffers.c.o.provides

src/buffers/CMakeFiles/shared_buffers.o.dir/shared_buffers.c.o.provides.build: src/buffers/CMakeFiles/shared_buffers.o.dir/shared_buffers.c.o


shared_buffers.o: src/buffers/CMakeFiles/shared_buffers.o.dir/shared_buffers.c.o
shared_buffers.o: src/buffers/CMakeFiles/shared_buffers.o.dir/build.make

.PHONY : shared_buffers.o

# Rule to build all files generated by this target.
src/buffers/CMakeFiles/shared_buffers.o.dir/build: shared_buffers.o

.PHONY : src/buffers/CMakeFiles/shared_buffers.o.dir/build

src/buffers/CMakeFiles/shared_buffers.o.dir/requires: src/buffers/CMakeFiles/shared_buffers.o.dir/shared_buffers.c.o.requires

.PHONY : src/buffers/CMakeFiles/shared_buffers.o.dir/requires

src/buffers/CMakeFiles/shared_buffers.o.dir/clean:
	cd /home/christian/CLionProjects/RESTProxy/src/buffers && $(CMAKE_COMMAND) -P CMakeFiles/shared_buffers.o.dir/cmake_clean.cmake
.PHONY : src/buffers/CMakeFiles/shared_buffers.o.dir/clean

src/buffers/CMakeFiles/shared_buffers.o.dir/depend:
	cd /home/christian/CLionProjects/RESTProxy && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/christian/CLionProjects/RESTProxy /home/christian/CLionProjects/RESTProxy/src/buffers /home/christian/CLionProjects/RESTProxy /home/christian/CLionProjects/RESTProxy/src/buffers /home/christian/CLionProjects/RESTProxy/src/buffers/CMakeFiles/shared_buffers.o.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/buffers/CMakeFiles/shared_buffers.o.dir/depend

