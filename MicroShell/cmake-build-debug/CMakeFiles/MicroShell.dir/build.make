# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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
CMAKE_COMMAND = /home/nikolai/Программы/clion-2020.1.2/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/nikolai/Программы/clion-2020.1.2/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/nikolai/CLionProjects/Fupm/MicroShell

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nikolai/CLionProjects/Fupm/MicroShell/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/MicroShell.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/MicroShell.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MicroShell.dir/flags.make

CMakeFiles/MicroShell.dir/main.cpp.o: CMakeFiles/MicroShell.dir/flags.make
CMakeFiles/MicroShell.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nikolai/CLionProjects/Fupm/MicroShell/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/MicroShell.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MicroShell.dir/main.cpp.o -c /home/nikolai/CLionProjects/Fupm/MicroShell/main.cpp

CMakeFiles/MicroShell.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MicroShell.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nikolai/CLionProjects/Fupm/MicroShell/main.cpp > CMakeFiles/MicroShell.dir/main.cpp.i

CMakeFiles/MicroShell.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MicroShell.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nikolai/CLionProjects/Fupm/MicroShell/main.cpp -o CMakeFiles/MicroShell.dir/main.cpp.s

# Object files for target MicroShell
MicroShell_OBJECTS = \
"CMakeFiles/MicroShell.dir/main.cpp.o"

# External object files for target MicroShell
MicroShell_EXTERNAL_OBJECTS =

MicroShell: CMakeFiles/MicroShell.dir/main.cpp.o
MicroShell: CMakeFiles/MicroShell.dir/build.make
MicroShell: CMakeFiles/MicroShell.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/nikolai/CLionProjects/Fupm/MicroShell/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable MicroShell"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MicroShell.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/MicroShell.dir/build: MicroShell

.PHONY : CMakeFiles/MicroShell.dir/build

CMakeFiles/MicroShell.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/MicroShell.dir/cmake_clean.cmake
.PHONY : CMakeFiles/MicroShell.dir/clean

CMakeFiles/MicroShell.dir/depend:
	cd /home/nikolai/CLionProjects/Fupm/MicroShell/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nikolai/CLionProjects/Fupm/MicroShell /home/nikolai/CLionProjects/Fupm/MicroShell /home/nikolai/CLionProjects/Fupm/MicroShell/cmake-build-debug /home/nikolai/CLionProjects/Fupm/MicroShell/cmake-build-debug /home/nikolai/CLionProjects/Fupm/MicroShell/cmake-build-debug/CMakeFiles/MicroShell.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/MicroShell.dir/depend
