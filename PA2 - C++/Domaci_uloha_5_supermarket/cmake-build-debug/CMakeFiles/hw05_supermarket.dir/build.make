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
CMAKE_SOURCE_DIR = /mnt/c/Users/marti/CLionProjects/hw05_supermarket

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/marti/CLionProjects/hw05_supermarket/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/hw05_supermarket.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/hw05_supermarket.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/hw05_supermarket.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/hw05_supermarket.dir/flags.make

CMakeFiles/hw05_supermarket.dir/main.cpp.o: CMakeFiles/hw05_supermarket.dir/flags.make
CMakeFiles/hw05_supermarket.dir/main.cpp.o: ../main.cpp
CMakeFiles/hw05_supermarket.dir/main.cpp.o: CMakeFiles/hw05_supermarket.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/marti/CLionProjects/hw05_supermarket/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/hw05_supermarket.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/hw05_supermarket.dir/main.cpp.o -MF CMakeFiles/hw05_supermarket.dir/main.cpp.o.d -o CMakeFiles/hw05_supermarket.dir/main.cpp.o -c /mnt/c/Users/marti/CLionProjects/hw05_supermarket/main.cpp

CMakeFiles/hw05_supermarket.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/hw05_supermarket.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/marti/CLionProjects/hw05_supermarket/main.cpp > CMakeFiles/hw05_supermarket.dir/main.cpp.i

CMakeFiles/hw05_supermarket.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/hw05_supermarket.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/marti/CLionProjects/hw05_supermarket/main.cpp -o CMakeFiles/hw05_supermarket.dir/main.cpp.s

# Object files for target hw05_supermarket
hw05_supermarket_OBJECTS = \
"CMakeFiles/hw05_supermarket.dir/main.cpp.o"

# External object files for target hw05_supermarket
hw05_supermarket_EXTERNAL_OBJECTS =

hw05_supermarket: CMakeFiles/hw05_supermarket.dir/main.cpp.o
hw05_supermarket: CMakeFiles/hw05_supermarket.dir/build.make
hw05_supermarket: CMakeFiles/hw05_supermarket.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/marti/CLionProjects/hw05_supermarket/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable hw05_supermarket"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/hw05_supermarket.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/hw05_supermarket.dir/build: hw05_supermarket
.PHONY : CMakeFiles/hw05_supermarket.dir/build

CMakeFiles/hw05_supermarket.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/hw05_supermarket.dir/cmake_clean.cmake
.PHONY : CMakeFiles/hw05_supermarket.dir/clean

CMakeFiles/hw05_supermarket.dir/depend:
	cd /mnt/c/Users/marti/CLionProjects/hw05_supermarket/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/marti/CLionProjects/hw05_supermarket /mnt/c/Users/marti/CLionProjects/hw05_supermarket /mnt/c/Users/marti/CLionProjects/hw05_supermarket/cmake-build-debug /mnt/c/Users/marti/CLionProjects/hw05_supermarket/cmake-build-debug /mnt/c/Users/marti/CLionProjects/hw05_supermarket/cmake-build-debug/CMakeFiles/hw05_supermarket.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/hw05_supermarket.dir/depend

