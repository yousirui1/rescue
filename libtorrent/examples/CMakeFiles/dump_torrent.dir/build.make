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
CMAKE_SOURCE_DIR = /home/ysr/project/libtorrent/examples

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ysr/project/libtorrent/examples

# Include any dependencies generated for this target.
include CMakeFiles/dump_torrent.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/dump_torrent.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/dump_torrent.dir/flags.make

CMakeFiles/dump_torrent.dir/dump_torrent.o: CMakeFiles/dump_torrent.dir/flags.make
CMakeFiles/dump_torrent.dir/dump_torrent.o: dump_torrent.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ysr/project/libtorrent/examples/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/dump_torrent.dir/dump_torrent.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/dump_torrent.dir/dump_torrent.o -c /home/ysr/project/libtorrent/examples/dump_torrent.cpp

CMakeFiles/dump_torrent.dir/dump_torrent.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dump_torrent.dir/dump_torrent.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ysr/project/libtorrent/examples/dump_torrent.cpp > CMakeFiles/dump_torrent.dir/dump_torrent.i

CMakeFiles/dump_torrent.dir/dump_torrent.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dump_torrent.dir/dump_torrent.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ysr/project/libtorrent/examples/dump_torrent.cpp -o CMakeFiles/dump_torrent.dir/dump_torrent.s

CMakeFiles/dump_torrent.dir/dump_torrent.o.requires:

.PHONY : CMakeFiles/dump_torrent.dir/dump_torrent.o.requires

CMakeFiles/dump_torrent.dir/dump_torrent.o.provides: CMakeFiles/dump_torrent.dir/dump_torrent.o.requires
	$(MAKE) -f CMakeFiles/dump_torrent.dir/build.make CMakeFiles/dump_torrent.dir/dump_torrent.o.provides.build
.PHONY : CMakeFiles/dump_torrent.dir/dump_torrent.o.provides

CMakeFiles/dump_torrent.dir/dump_torrent.o.provides.build: CMakeFiles/dump_torrent.dir/dump_torrent.o


# Object files for target dump_torrent
dump_torrent_OBJECTS = \
"CMakeFiles/dump_torrent.dir/dump_torrent.o"

# External object files for target dump_torrent
dump_torrent_EXTERNAL_OBJECTS =

dump_torrent: CMakeFiles/dump_torrent.dir/dump_torrent.o
dump_torrent: CMakeFiles/dump_torrent.dir/build.make
dump_torrent: CMakeFiles/dump_torrent.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ysr/project/libtorrent/examples/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable dump_torrent"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/dump_torrent.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/dump_torrent.dir/build: dump_torrent

.PHONY : CMakeFiles/dump_torrent.dir/build

CMakeFiles/dump_torrent.dir/requires: CMakeFiles/dump_torrent.dir/dump_torrent.o.requires

.PHONY : CMakeFiles/dump_torrent.dir/requires

CMakeFiles/dump_torrent.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/dump_torrent.dir/cmake_clean.cmake
.PHONY : CMakeFiles/dump_torrent.dir/clean

CMakeFiles/dump_torrent.dir/depend:
	cd /home/ysr/project/libtorrent/examples && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ysr/project/libtorrent/examples /home/ysr/project/libtorrent/examples /home/ysr/project/libtorrent/examples /home/ysr/project/libtorrent/examples /home/ysr/project/libtorrent/examples/CMakeFiles/dump_torrent.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/dump_torrent.dir/depend

