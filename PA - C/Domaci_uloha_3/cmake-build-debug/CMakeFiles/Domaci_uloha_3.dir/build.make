# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.20

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2021.2.1\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2021.2.1\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\marti\CLionProjects\Domaci_uloha_3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\marti\CLionProjects\Domaci_uloha_3\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Domaci_uloha_3.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/Domaci_uloha_3.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Domaci_uloha_3.dir/flags.make

CMakeFiles/Domaci_uloha_3.dir/main.c.obj: CMakeFiles/Domaci_uloha_3.dir/flags.make
CMakeFiles/Domaci_uloha_3.dir/main.c.obj: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\marti\CLionProjects\Domaci_uloha_3\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Domaci_uloha_3.dir/main.c.obj"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\Domaci_uloha_3.dir\main.c.obj -c C:\Users\marti\CLionProjects\Domaci_uloha_3\main.c

CMakeFiles/Domaci_uloha_3.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Domaci_uloha_3.dir/main.c.i"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\marti\CLionProjects\Domaci_uloha_3\main.c > CMakeFiles\Domaci_uloha_3.dir\main.c.i

CMakeFiles/Domaci_uloha_3.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Domaci_uloha_3.dir/main.c.s"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\marti\CLionProjects\Domaci_uloha_3\main.c -o CMakeFiles\Domaci_uloha_3.dir\main.c.s

# Object files for target Domaci_uloha_3
Domaci_uloha_3_OBJECTS = \
"CMakeFiles/Domaci_uloha_3.dir/main.c.obj"

# External object files for target Domaci_uloha_3
Domaci_uloha_3_EXTERNAL_OBJECTS =

Domaci_uloha_3.exe: CMakeFiles/Domaci_uloha_3.dir/main.c.obj
Domaci_uloha_3.exe: CMakeFiles/Domaci_uloha_3.dir/build.make
Domaci_uloha_3.exe: CMakeFiles/Domaci_uloha_3.dir/linklibs.rsp
Domaci_uloha_3.exe: CMakeFiles/Domaci_uloha_3.dir/objects1.rsp
Domaci_uloha_3.exe: CMakeFiles/Domaci_uloha_3.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\marti\CLionProjects\Domaci_uloha_3\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable Domaci_uloha_3.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\Domaci_uloha_3.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Domaci_uloha_3.dir/build: Domaci_uloha_3.exe
.PHONY : CMakeFiles/Domaci_uloha_3.dir/build

CMakeFiles/Domaci_uloha_3.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\Domaci_uloha_3.dir\cmake_clean.cmake
.PHONY : CMakeFiles/Domaci_uloha_3.dir/clean

CMakeFiles/Domaci_uloha_3.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\marti\CLionProjects\Domaci_uloha_3 C:\Users\marti\CLionProjects\Domaci_uloha_3 C:\Users\marti\CLionProjects\Domaci_uloha_3\cmake-build-debug C:\Users\marti\CLionProjects\Domaci_uloha_3\cmake-build-debug C:\Users\marti\CLionProjects\Domaci_uloha_3\cmake-build-debug\CMakeFiles\Domaci_uloha_3.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Domaci_uloha_3.dir/depend

