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
CMAKE_SOURCE_DIR = /tmp/tmp.loTla5Bejk

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /tmp/tmp.loTla5Bejk/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/lscode.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/lscode.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/lscode.dir/flags.make

CMakeFiles/lscode.dir/MemoryPool.cpp.o: CMakeFiles/lscode.dir/flags.make
CMakeFiles/lscode.dir/MemoryPool.cpp.o: ../MemoryPool.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.loTla5Bejk/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/lscode.dir/MemoryPool.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/lscode.dir/MemoryPool.cpp.o -c /tmp/tmp.loTla5Bejk/MemoryPool.cpp

CMakeFiles/lscode.dir/MemoryPool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lscode.dir/MemoryPool.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.loTla5Bejk/MemoryPool.cpp > CMakeFiles/lscode.dir/MemoryPool.cpp.i

CMakeFiles/lscode.dir/MemoryPool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lscode.dir/MemoryPool.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.loTla5Bejk/MemoryPool.cpp -o CMakeFiles/lscode.dir/MemoryPool.cpp.s

CMakeFiles/lscode.dir/LockFreeQueue.cpp.o: CMakeFiles/lscode.dir/flags.make
CMakeFiles/lscode.dir/LockFreeQueue.cpp.o: ../LockFreeQueue.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.loTla5Bejk/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/lscode.dir/LockFreeQueue.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/lscode.dir/LockFreeQueue.cpp.o -c /tmp/tmp.loTla5Bejk/LockFreeQueue.cpp

CMakeFiles/lscode.dir/LockFreeQueue.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lscode.dir/LockFreeQueue.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.loTla5Bejk/LockFreeQueue.cpp > CMakeFiles/lscode.dir/LockFreeQueue.cpp.i

CMakeFiles/lscode.dir/LockFreeQueue.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lscode.dir/LockFreeQueue.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.loTla5Bejk/LockFreeQueue.cpp -o CMakeFiles/lscode.dir/LockFreeQueue.cpp.s

# Object files for target lscode
lscode_OBJECTS = \
"CMakeFiles/lscode.dir/MemoryPool.cpp.o" \
"CMakeFiles/lscode.dir/LockFreeQueue.cpp.o"

# External object files for target lscode
lscode_EXTERNAL_OBJECTS =

lscode: CMakeFiles/lscode.dir/MemoryPool.cpp.o
lscode: CMakeFiles/lscode.dir/LockFreeQueue.cpp.o
lscode: CMakeFiles/lscode.dir/build.make
lscode: CMakeFiles/lscode.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/tmp/tmp.loTla5Bejk/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable lscode"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/lscode.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/lscode.dir/build: lscode

.PHONY : CMakeFiles/lscode.dir/build

CMakeFiles/lscode.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/lscode.dir/cmake_clean.cmake
.PHONY : CMakeFiles/lscode.dir/clean

CMakeFiles/lscode.dir/depend:
	cd /tmp/tmp.loTla5Bejk/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /tmp/tmp.loTla5Bejk /tmp/tmp.loTla5Bejk /tmp/tmp.loTla5Bejk/cmake-build-debug /tmp/tmp.loTla5Bejk/cmake-build-debug /tmp/tmp.loTla5Bejk/cmake-build-debug/CMakeFiles/lscode.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/lscode.dir/depend

