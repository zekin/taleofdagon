# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_SOURCE_DIR = /home/zekin/projects/C009-TaleOfRedengard

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zekin/projects/C009-TaleOfRedengard/build

# Include any dependencies generated for this target.
include CMakeFiles/c009-taleofredengard.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/c009-taleofredengard.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/c009-taleofredengard.dir/flags.make

CMakeFiles/c009-taleofredengard.dir/main.o: CMakeFiles/c009-taleofredengard.dir/flags.make
CMakeFiles/c009-taleofredengard.dir/main.o: ../main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/zekin/projects/C009-TaleOfRedengard/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/c009-taleofredengard.dir/main.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/c009-taleofredengard.dir/main.o -c /home/zekin/projects/C009-TaleOfRedengard/main.cpp

CMakeFiles/c009-taleofredengard.dir/main.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/c009-taleofredengard.dir/main.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/zekin/projects/C009-TaleOfRedengard/main.cpp > CMakeFiles/c009-taleofredengard.dir/main.i

CMakeFiles/c009-taleofredengard.dir/main.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/c009-taleofredengard.dir/main.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/zekin/projects/C009-TaleOfRedengard/main.cpp -o CMakeFiles/c009-taleofredengard.dir/main.s

CMakeFiles/c009-taleofredengard.dir/main.o.requires:
.PHONY : CMakeFiles/c009-taleofredengard.dir/main.o.requires

CMakeFiles/c009-taleofredengard.dir/main.o.provides: CMakeFiles/c009-taleofredengard.dir/main.o.requires
	$(MAKE) -f CMakeFiles/c009-taleofredengard.dir/build.make CMakeFiles/c009-taleofredengard.dir/main.o.provides.build
.PHONY : CMakeFiles/c009-taleofredengard.dir/main.o.provides

CMakeFiles/c009-taleofredengard.dir/main.o.provides.build: CMakeFiles/c009-taleofredengard.dir/main.o

# Object files for target c009-taleofredengard
c009__taleofredengard_OBJECTS = \
"CMakeFiles/c009-taleofredengard.dir/main.o"

# External object files for target c009-taleofredengard
c009__taleofredengard_EXTERNAL_OBJECTS =

c009-taleofredengard: CMakeFiles/c009-taleofredengard.dir/main.o
c009-taleofredengard: CMakeFiles/c009-taleofredengard.dir/build.make
c009-taleofredengard: /usr/lib/i386-linux-gnu/libSDLmain.a
c009-taleofredengard: /usr/lib/i386-linux-gnu/libSDL.so
c009-taleofredengard: /usr/lib/i386-linux-gnu/libSDL_mixer.so
c009-taleofredengard: /usr/lib/i386-linux-gnu/libSDL_image.so
c009-taleofredengard: /usr/lib/i386-linux-gnu/libGLU.so
c009-taleofredengard: /usr/lib/i386-linux-gnu/libGL.so
c009-taleofredengard: /usr/lib/i386-linux-gnu/libSM.so
c009-taleofredengard: /usr/lib/i386-linux-gnu/libICE.so
c009-taleofredengard: /usr/lib/i386-linux-gnu/libX11.so
c009-taleofredengard: /usr/lib/i386-linux-gnu/libXext.so
c009-taleofredengard: CMakeFiles/c009-taleofredengard.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable c009-taleofredengard"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/c009-taleofredengard.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/c009-taleofredengard.dir/build: c009-taleofredengard
.PHONY : CMakeFiles/c009-taleofredengard.dir/build

CMakeFiles/c009-taleofredengard.dir/requires: CMakeFiles/c009-taleofredengard.dir/main.o.requires
.PHONY : CMakeFiles/c009-taleofredengard.dir/requires

CMakeFiles/c009-taleofredengard.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/c009-taleofredengard.dir/cmake_clean.cmake
.PHONY : CMakeFiles/c009-taleofredengard.dir/clean

CMakeFiles/c009-taleofredengard.dir/depend:
	cd /home/zekin/projects/C009-TaleOfRedengard/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zekin/projects/C009-TaleOfRedengard /home/zekin/projects/C009-TaleOfRedengard /home/zekin/projects/C009-TaleOfRedengard/build /home/zekin/projects/C009-TaleOfRedengard/build /home/zekin/projects/C009-TaleOfRedengard/build/CMakeFiles/c009-taleofredengard.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/c009-taleofredengard.dir/depend
