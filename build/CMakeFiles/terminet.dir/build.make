# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

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
CMAKE_SOURCE_DIR = /home/rgb/Desktop/projects/terminet_backend

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/rgb/Desktop/projects/terminet_backend/build

# Include any dependencies generated for this target.
include CMakeFiles/terminet.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/terminet.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/terminet.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/terminet.dir/flags.make

CMakeFiles/terminet.dir/codegen:
.PHONY : CMakeFiles/terminet.dir/codegen

CMakeFiles/terminet.dir/main.cpp.o: CMakeFiles/terminet.dir/flags.make
CMakeFiles/terminet.dir/main.cpp.o: /home/rgb/Desktop/projects/terminet_backend/main.cpp
CMakeFiles/terminet.dir/main.cpp.o: CMakeFiles/terminet.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/rgb/Desktop/projects/terminet_backend/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/terminet.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/terminet.dir/main.cpp.o -MF CMakeFiles/terminet.dir/main.cpp.o.d -o CMakeFiles/terminet.dir/main.cpp.o -c /home/rgb/Desktop/projects/terminet_backend/main.cpp

CMakeFiles/terminet.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/terminet.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rgb/Desktop/projects/terminet_backend/main.cpp > CMakeFiles/terminet.dir/main.cpp.i

CMakeFiles/terminet.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/terminet.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rgb/Desktop/projects/terminet_backend/main.cpp -o CMakeFiles/terminet.dir/main.cpp.s

# Object files for target terminet
terminet_OBJECTS = \
"CMakeFiles/terminet.dir/main.cpp.o"

# External object files for target terminet
terminet_EXTERNAL_OBJECTS =

terminet: CMakeFiles/terminet.dir/main.cpp.o
terminet: CMakeFiles/terminet.dir/build.make
terminet: CMakeFiles/terminet.dir/compiler_depend.ts
terminet: libconnection_manager.a
terminet: liblogin_repo.a
terminet: liblogin_service.a
terminet: libapp_helper.a
terminet: liblogin_repo.a
terminet: libconnection_manager.a
terminet: CMakeFiles/terminet.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/rgb/Desktop/projects/terminet_backend/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable terminet"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/terminet.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/terminet.dir/build: terminet
.PHONY : CMakeFiles/terminet.dir/build

CMakeFiles/terminet.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/terminet.dir/cmake_clean.cmake
.PHONY : CMakeFiles/terminet.dir/clean

CMakeFiles/terminet.dir/depend:
	cd /home/rgb/Desktop/projects/terminet_backend/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/rgb/Desktop/projects/terminet_backend /home/rgb/Desktop/projects/terminet_backend /home/rgb/Desktop/projects/terminet_backend/build /home/rgb/Desktop/projects/terminet_backend/build /home/rgb/Desktop/projects/terminet_backend/build/CMakeFiles/terminet.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/terminet.dir/depend

