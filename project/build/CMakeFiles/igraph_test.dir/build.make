# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/atomesz/qfs/VLG/project

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/atomesz/qfs/VLG/project/build

# Include any dependencies generated for this target.
include CMakeFiles/igraph_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/igraph_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/igraph_test.dir/flags.make

CMakeFiles/igraph_test.dir/igraph_test.c.o: CMakeFiles/igraph_test.dir/flags.make
CMakeFiles/igraph_test.dir/igraph_test.c.o: ../igraph_test.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/atomesz/qfs/VLG/project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/igraph_test.dir/igraph_test.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/igraph_test.dir/igraph_test.c.o   -c /home/atomesz/qfs/VLG/project/igraph_test.c

CMakeFiles/igraph_test.dir/igraph_test.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/igraph_test.dir/igraph_test.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/atomesz/qfs/VLG/project/igraph_test.c > CMakeFiles/igraph_test.dir/igraph_test.c.i

CMakeFiles/igraph_test.dir/igraph_test.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/igraph_test.dir/igraph_test.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/atomesz/qfs/VLG/project/igraph_test.c -o CMakeFiles/igraph_test.dir/igraph_test.c.s

# Object files for target igraph_test
igraph_test_OBJECTS = \
"CMakeFiles/igraph_test.dir/igraph_test.c.o"

# External object files for target igraph_test
igraph_test_EXTERNAL_OBJECTS =

igraph_test: CMakeFiles/igraph_test.dir/igraph_test.c.o
igraph_test: CMakeFiles/igraph_test.dir/build.make
igraph_test: /usr/local/lib/libigraph.a
igraph_test: /usr/lib/x86_64-linux-gnu/libm.so
igraph_test: /usr/lib/x86_64-linux-gnu/libblas.so
igraph_test: /usr/lib/x86_64-linux-gnu/liblapack.so
igraph_test: /usr/lib/x86_64-linux-gnu/libblas.so
igraph_test: /usr/lib/x86_64-linux-gnu/liblapack.so
igraph_test: /usr/lib/gcc/x86_64-linux-gnu/9/libgomp.so
igraph_test: /usr/lib/x86_64-linux-gnu/libpthread.so
igraph_test: CMakeFiles/igraph_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/atomesz/qfs/VLG/project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable igraph_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/igraph_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/igraph_test.dir/build: igraph_test

.PHONY : CMakeFiles/igraph_test.dir/build

CMakeFiles/igraph_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/igraph_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/igraph_test.dir/clean

CMakeFiles/igraph_test.dir/depend:
	cd /home/atomesz/qfs/VLG/project/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/atomesz/qfs/VLG/project /home/atomesz/qfs/VLG/project /home/atomesz/qfs/VLG/project/build /home/atomesz/qfs/VLG/project/build /home/atomesz/qfs/VLG/project/build/CMakeFiles/igraph_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/igraph_test.dir/depend
