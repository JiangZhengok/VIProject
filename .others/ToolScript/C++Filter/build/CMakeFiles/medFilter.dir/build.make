# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/bit/data/my_tool/C++Filter

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bit/data/my_tool/C++Filter/build

# Include any dependencies generated for this target.
include CMakeFiles/medFilter.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/medFilter.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/medFilter.dir/flags.make

CMakeFiles/medFilter.dir/medFilter.cpp.o: CMakeFiles/medFilter.dir/flags.make
CMakeFiles/medFilter.dir/medFilter.cpp.o: ../medFilter.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bit/data/my_tool/C++Filter/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/medFilter.dir/medFilter.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/medFilter.dir/medFilter.cpp.o -c /home/bit/data/my_tool/C++Filter/medFilter.cpp

CMakeFiles/medFilter.dir/medFilter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/medFilter.dir/medFilter.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bit/data/my_tool/C++Filter/medFilter.cpp > CMakeFiles/medFilter.dir/medFilter.cpp.i

CMakeFiles/medFilter.dir/medFilter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/medFilter.dir/medFilter.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bit/data/my_tool/C++Filter/medFilter.cpp -o CMakeFiles/medFilter.dir/medFilter.cpp.s

CMakeFiles/medFilter.dir/medFilter.cpp.o.requires:

.PHONY : CMakeFiles/medFilter.dir/medFilter.cpp.o.requires

CMakeFiles/medFilter.dir/medFilter.cpp.o.provides: CMakeFiles/medFilter.dir/medFilter.cpp.o.requires
	$(MAKE) -f CMakeFiles/medFilter.dir/build.make CMakeFiles/medFilter.dir/medFilter.cpp.o.provides.build
.PHONY : CMakeFiles/medFilter.dir/medFilter.cpp.o.provides

CMakeFiles/medFilter.dir/medFilter.cpp.o.provides.build: CMakeFiles/medFilter.dir/medFilter.cpp.o


# Object files for target medFilter
medFilter_OBJECTS = \
"CMakeFiles/medFilter.dir/medFilter.cpp.o"

# External object files for target medFilter
medFilter_EXTERNAL_OBJECTS =

medFilter: CMakeFiles/medFilter.dir/medFilter.cpp.o
medFilter: CMakeFiles/medFilter.dir/build.make
medFilter: CMakeFiles/medFilter.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bit/data/my_tool/C++Filter/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable medFilter"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/medFilter.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/medFilter.dir/build: medFilter

.PHONY : CMakeFiles/medFilter.dir/build

CMakeFiles/medFilter.dir/requires: CMakeFiles/medFilter.dir/medFilter.cpp.o.requires

.PHONY : CMakeFiles/medFilter.dir/requires

CMakeFiles/medFilter.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/medFilter.dir/cmake_clean.cmake
.PHONY : CMakeFiles/medFilter.dir/clean

CMakeFiles/medFilter.dir/depend:
	cd /home/bit/data/my_tool/C++Filter/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bit/data/my_tool/C++Filter /home/bit/data/my_tool/C++Filter /home/bit/data/my_tool/C++Filter/build /home/bit/data/my_tool/C++Filter/build /home/bit/data/my_tool/C++Filter/build/CMakeFiles/medFilter.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/medFilter.dir/depend

