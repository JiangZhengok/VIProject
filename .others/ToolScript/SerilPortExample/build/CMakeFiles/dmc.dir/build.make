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
CMAKE_SOURCE_DIR = /home/bit/my/serilport

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bit/my/serilport/build

# Include any dependencies generated for this target.
include CMakeFiles/dmc.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/dmc.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/dmc.dir/flags.make

CMakeFiles/dmc.dir/src/DMC.c.o: CMakeFiles/dmc.dir/flags.make
CMakeFiles/dmc.dir/src/DMC.c.o: ../src/DMC.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bit/my/serilport/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/dmc.dir/src/DMC.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/dmc.dir/src/DMC.c.o   -c /home/bit/my/serilport/src/DMC.c

CMakeFiles/dmc.dir/src/DMC.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dmc.dir/src/DMC.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bit/my/serilport/src/DMC.c > CMakeFiles/dmc.dir/src/DMC.c.i

CMakeFiles/dmc.dir/src/DMC.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dmc.dir/src/DMC.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bit/my/serilport/src/DMC.c -o CMakeFiles/dmc.dir/src/DMC.c.s

CMakeFiles/dmc.dir/src/DMC.c.o.requires:

.PHONY : CMakeFiles/dmc.dir/src/DMC.c.o.requires

CMakeFiles/dmc.dir/src/DMC.c.o.provides: CMakeFiles/dmc.dir/src/DMC.c.o.requires
	$(MAKE) -f CMakeFiles/dmc.dir/build.make CMakeFiles/dmc.dir/src/DMC.c.o.provides.build
.PHONY : CMakeFiles/dmc.dir/src/DMC.c.o.provides

CMakeFiles/dmc.dir/src/DMC.c.o.provides.build: CMakeFiles/dmc.dir/src/DMC.c.o


# Object files for target dmc
dmc_OBJECTS = \
"CMakeFiles/dmc.dir/src/DMC.c.o"

# External object files for target dmc
dmc_EXTERNAL_OBJECTS =

dmc: CMakeFiles/dmc.dir/src/DMC.c.o
dmc: CMakeFiles/dmc.dir/build.make
dmc: CMakeFiles/dmc.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bit/my/serilport/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable dmc"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/dmc.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/dmc.dir/build: dmc

.PHONY : CMakeFiles/dmc.dir/build

CMakeFiles/dmc.dir/requires: CMakeFiles/dmc.dir/src/DMC.c.o.requires

.PHONY : CMakeFiles/dmc.dir/requires

CMakeFiles/dmc.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/dmc.dir/cmake_clean.cmake
.PHONY : CMakeFiles/dmc.dir/clean

CMakeFiles/dmc.dir/depend:
	cd /home/bit/my/serilport/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bit/my/serilport /home/bit/my/serilport /home/bit/my/serilport/build /home/bit/my/serilport/build /home/bit/my/serilport/build/CMakeFiles/dmc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/dmc.dir/depend

