# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/c/Users/marti/CLionProjects/hw02_tridy

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/marti/CLionProjects/hw02_tridy/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/hw02_tridy.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/hw02_tridy.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/hw02_tridy.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/hw02_tridy.dir/flags.make

CMakeFiles/hw02_tridy.dir/main.cpp.o: CMakeFiles/hw02_tridy.dir/flags.make
CMakeFiles/hw02_tridy.dir/main.cpp.o: ../main.cpp
CMakeFiles/hw02_tridy.dir/main.cpp.o: CMakeFiles/hw02_tridy.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/marti/CLionProjects/hw02_tridy/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/hw02_tridy.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/hw02_tridy.dir/main.cpp.o -MF CMakeFiles/hw02_tridy.dir/main.cpp.o.d -o CMakeFiles/hw02_tridy.dir/main.cpp.o -c /mnt/c/Users/marti/CLionProjects/hw02_tridy/main.cpp

CMakeFiles/hw02_tridy.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/hw02_tridy.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/marti/CLionProjects/hw02_tridy/main.cpp > CMakeFiles/hw02_tridy.dir/main.cpp.i

CMakeFiles/hw02_tridy.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/hw02_tridy.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/marti/CLionProjects/hw02_tridy/main.cpp -o CMakeFiles/hw02_tridy.dir/main.cpp.s

# Object files for target hw02_tridy
hw02_tridy_OBJECTS = \
"CMakeFiles/hw02_tridy.dir/main.cpp.o"

# External object files for target hw02_tridy
hw02_tridy_EXTERNAL_OBJECTS =

hw02_tridy: CMakeFiles/hw02_tridy.dir/main.cpp.o
hw02_tridy: CMakeFiles/hw02_tridy.dir/build.make
hw02_tridy: CMakeFiles/hw02_tridy.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/marti/CLionProjects/hw02_tridy/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable hw02_tridy"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/hw02_tridy.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/hw02_tridy.dir/build: hw02_tridy
.PHONY : CMakeFiles/hw02_tridy.dir/build

CMakeFiles/hw02_tridy.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/hw02_tridy.dir/cmake_clean.cmake
.PHONY : CMakeFiles/hw02_tridy.dir/clean

CMakeFiles/hw02_tridy.dir/depend:
	cd /mnt/c/Users/marti/CLionProjects/hw02_tridy/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/marti/CLionProjects/hw02_tridy /mnt/c/Users/marti/CLionProjects/hw02_tridy /mnt/c/Users/marti/CLionProjects/hw02_tridy/cmake-build-debug /mnt/c/Users/marti/CLionProjects/hw02_tridy/cmake-build-debug /mnt/c/Users/marti/CLionProjects/hw02_tridy/cmake-build-debug/CMakeFiles/hw02_tridy.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/hw02_tridy.dir/depend

