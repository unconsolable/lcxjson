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
CMAKE_SOURCE_DIR = /home/chen/projects/lcxjson

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/chen/projects/lcxjson/build

# Include any dependencies generated for this target.
include CMakeFiles/lcxjson_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/lcxjson_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/lcxjson_test.dir/flags.make

CMakeFiles/lcxjson_test.dir/test.cc.o: CMakeFiles/lcxjson_test.dir/flags.make
CMakeFiles/lcxjson_test.dir/test.cc.o: ../test.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chen/projects/lcxjson/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/lcxjson_test.dir/test.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/lcxjson_test.dir/test.cc.o -c /home/chen/projects/lcxjson/test.cc

CMakeFiles/lcxjson_test.dir/test.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lcxjson_test.dir/test.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chen/projects/lcxjson/test.cc > CMakeFiles/lcxjson_test.dir/test.cc.i

CMakeFiles/lcxjson_test.dir/test.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lcxjson_test.dir/test.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chen/projects/lcxjson/test.cc -o CMakeFiles/lcxjson_test.dir/test.cc.s

CMakeFiles/lcxjson_test.dir/test.cc.o.requires:

.PHONY : CMakeFiles/lcxjson_test.dir/test.cc.o.requires

CMakeFiles/lcxjson_test.dir/test.cc.o.provides: CMakeFiles/lcxjson_test.dir/test.cc.o.requires
	$(MAKE) -f CMakeFiles/lcxjson_test.dir/build.make CMakeFiles/lcxjson_test.dir/test.cc.o.provides.build
.PHONY : CMakeFiles/lcxjson_test.dir/test.cc.o.provides

CMakeFiles/lcxjson_test.dir/test.cc.o.provides.build: CMakeFiles/lcxjson_test.dir/test.cc.o


# Object files for target lcxjson_test
lcxjson_test_OBJECTS = \
"CMakeFiles/lcxjson_test.dir/test.cc.o"

# External object files for target lcxjson_test
lcxjson_test_EXTERNAL_OBJECTS =

lcxjson_test: CMakeFiles/lcxjson_test.dir/test.cc.o
lcxjson_test: CMakeFiles/lcxjson_test.dir/build.make
lcxjson_test: liblcxjson.a
lcxjson_test: CMakeFiles/lcxjson_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/chen/projects/lcxjson/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable lcxjson_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/lcxjson_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/lcxjson_test.dir/build: lcxjson_test

.PHONY : CMakeFiles/lcxjson_test.dir/build

CMakeFiles/lcxjson_test.dir/requires: CMakeFiles/lcxjson_test.dir/test.cc.o.requires

.PHONY : CMakeFiles/lcxjson_test.dir/requires

CMakeFiles/lcxjson_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/lcxjson_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/lcxjson_test.dir/clean

CMakeFiles/lcxjson_test.dir/depend:
	cd /home/chen/projects/lcxjson/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chen/projects/lcxjson /home/chen/projects/lcxjson /home/chen/projects/lcxjson/build /home/chen/projects/lcxjson/build /home/chen/projects/lcxjson/build/CMakeFiles/lcxjson_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/lcxjson_test.dir/depend

