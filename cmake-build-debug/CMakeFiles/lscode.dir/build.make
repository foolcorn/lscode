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
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2021.2\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2021.2\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\lscode

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\lscode\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/lscode.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/lscode.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/lscode.dir/flags.make

CMakeFiles/lscode.dir/BTree.cpp.obj: CMakeFiles/lscode.dir/flags.make
CMakeFiles/lscode.dir/BTree.cpp.obj: ../BTree.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\lscode\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/lscode.dir/BTree.cpp.obj"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\lscode.dir\BTree.cpp.obj -c D:\lscode\BTree.cpp

CMakeFiles/lscode.dir/BTree.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lscode.dir/BTree.cpp.i"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\lscode\BTree.cpp > CMakeFiles\lscode.dir\BTree.cpp.i

CMakeFiles/lscode.dir/BTree.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lscode.dir/BTree.cpp.s"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\lscode\BTree.cpp -o CMakeFiles\lscode.dir\BTree.cpp.s

CMakeFiles/lscode.dir/design-pattern/singleton.cpp.obj: CMakeFiles/lscode.dir/flags.make
CMakeFiles/lscode.dir/design-pattern/singleton.cpp.obj: ../design-pattern/singleton.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\lscode\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/lscode.dir/design-pattern/singleton.cpp.obj"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\lscode.dir\design-pattern\singleton.cpp.obj -c D:\lscode\design-pattern\singleton.cpp

CMakeFiles/lscode.dir/design-pattern/singleton.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lscode.dir/design-pattern/singleton.cpp.i"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\lscode\design-pattern\singleton.cpp > CMakeFiles\lscode.dir\design-pattern\singleton.cpp.i

CMakeFiles/lscode.dir/design-pattern/singleton.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lscode.dir/design-pattern/singleton.cpp.s"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\lscode\design-pattern\singleton.cpp -o CMakeFiles\lscode.dir\design-pattern\singleton.cpp.s

# Object files for target lscode
lscode_OBJECTS = \
"CMakeFiles/lscode.dir/BTree.cpp.obj" \
"CMakeFiles/lscode.dir/design-pattern/singleton.cpp.obj"

# External object files for target lscode
lscode_EXTERNAL_OBJECTS =

lscode.exe: CMakeFiles/lscode.dir/BTree.cpp.obj
lscode.exe: CMakeFiles/lscode.dir/design-pattern/singleton.cpp.obj
lscode.exe: CMakeFiles/lscode.dir/build.make
lscode.exe: CMakeFiles/lscode.dir/linklibs.rsp
lscode.exe: CMakeFiles/lscode.dir/objects1.rsp
lscode.exe: CMakeFiles/lscode.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\lscode\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable lscode.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\lscode.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/lscode.dir/build: lscode.exe
.PHONY : CMakeFiles/lscode.dir/build

CMakeFiles/lscode.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\lscode.dir\cmake_clean.cmake
.PHONY : CMakeFiles/lscode.dir/clean

CMakeFiles/lscode.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\lscode D:\lscode D:\lscode\cmake-build-debug D:\lscode\cmake-build-debug D:\lscode\cmake-build-debug\CMakeFiles\lscode.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/lscode.dir/depend

