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
include CMakeFiles/client_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/client_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/client_test.dir/flags.make

CMakeFiles/client_test.dir/client_test.o: CMakeFiles/client_test.dir/flags.make
CMakeFiles/client_test.dir/client_test.o: client_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ysr/project/libtorrent/examples/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/client_test.dir/client_test.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client_test.dir/client_test.o -c /home/ysr/project/libtorrent/examples/client_test.cpp

CMakeFiles/client_test.dir/client_test.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client_test.dir/client_test.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ysr/project/libtorrent/examples/client_test.cpp > CMakeFiles/client_test.dir/client_test.i

CMakeFiles/client_test.dir/client_test.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client_test.dir/client_test.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ysr/project/libtorrent/examples/client_test.cpp -o CMakeFiles/client_test.dir/client_test.s

CMakeFiles/client_test.dir/client_test.o.requires:

.PHONY : CMakeFiles/client_test.dir/client_test.o.requires

CMakeFiles/client_test.dir/client_test.o.provides: CMakeFiles/client_test.dir/client_test.o.requires
	$(MAKE) -f CMakeFiles/client_test.dir/build.make CMakeFiles/client_test.dir/client_test.o.provides.build
.PHONY : CMakeFiles/client_test.dir/client_test.o.provides

CMakeFiles/client_test.dir/client_test.o.provides.build: CMakeFiles/client_test.dir/client_test.o


CMakeFiles/client_test.dir/print.o: CMakeFiles/client_test.dir/flags.make
CMakeFiles/client_test.dir/print.o: print.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ysr/project/libtorrent/examples/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/client_test.dir/print.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client_test.dir/print.o -c /home/ysr/project/libtorrent/examples/print.cpp

CMakeFiles/client_test.dir/print.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client_test.dir/print.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ysr/project/libtorrent/examples/print.cpp > CMakeFiles/client_test.dir/print.i

CMakeFiles/client_test.dir/print.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client_test.dir/print.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ysr/project/libtorrent/examples/print.cpp -o CMakeFiles/client_test.dir/print.s

CMakeFiles/client_test.dir/print.o.requires:

.PHONY : CMakeFiles/client_test.dir/print.o.requires

CMakeFiles/client_test.dir/print.o.provides: CMakeFiles/client_test.dir/print.o.requires
	$(MAKE) -f CMakeFiles/client_test.dir/build.make CMakeFiles/client_test.dir/print.o.provides.build
.PHONY : CMakeFiles/client_test.dir/print.o.provides

CMakeFiles/client_test.dir/print.o.provides.build: CMakeFiles/client_test.dir/print.o


CMakeFiles/client_test.dir/torrent_view.o: CMakeFiles/client_test.dir/flags.make
CMakeFiles/client_test.dir/torrent_view.o: torrent_view.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ysr/project/libtorrent/examples/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/client_test.dir/torrent_view.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client_test.dir/torrent_view.o -c /home/ysr/project/libtorrent/examples/torrent_view.cpp

CMakeFiles/client_test.dir/torrent_view.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client_test.dir/torrent_view.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ysr/project/libtorrent/examples/torrent_view.cpp > CMakeFiles/client_test.dir/torrent_view.i

CMakeFiles/client_test.dir/torrent_view.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client_test.dir/torrent_view.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ysr/project/libtorrent/examples/torrent_view.cpp -o CMakeFiles/client_test.dir/torrent_view.s

CMakeFiles/client_test.dir/torrent_view.o.requires:

.PHONY : CMakeFiles/client_test.dir/torrent_view.o.requires

CMakeFiles/client_test.dir/torrent_view.o.provides: CMakeFiles/client_test.dir/torrent_view.o.requires
	$(MAKE) -f CMakeFiles/client_test.dir/build.make CMakeFiles/client_test.dir/torrent_view.o.provides.build
.PHONY : CMakeFiles/client_test.dir/torrent_view.o.provides

CMakeFiles/client_test.dir/torrent_view.o.provides.build: CMakeFiles/client_test.dir/torrent_view.o


CMakeFiles/client_test.dir/session_view.o: CMakeFiles/client_test.dir/flags.make
CMakeFiles/client_test.dir/session_view.o: session_view.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ysr/project/libtorrent/examples/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/client_test.dir/session_view.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client_test.dir/session_view.o -c /home/ysr/project/libtorrent/examples/session_view.cpp

CMakeFiles/client_test.dir/session_view.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client_test.dir/session_view.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ysr/project/libtorrent/examples/session_view.cpp > CMakeFiles/client_test.dir/session_view.i

CMakeFiles/client_test.dir/session_view.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client_test.dir/session_view.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ysr/project/libtorrent/examples/session_view.cpp -o CMakeFiles/client_test.dir/session_view.s

CMakeFiles/client_test.dir/session_view.o.requires:

.PHONY : CMakeFiles/client_test.dir/session_view.o.requires

CMakeFiles/client_test.dir/session_view.o.provides: CMakeFiles/client_test.dir/session_view.o.requires
	$(MAKE) -f CMakeFiles/client_test.dir/build.make CMakeFiles/client_test.dir/session_view.o.provides.build
.PHONY : CMakeFiles/client_test.dir/session_view.o.provides

CMakeFiles/client_test.dir/session_view.o.provides.build: CMakeFiles/client_test.dir/session_view.o


# Object files for target client_test
client_test_OBJECTS = \
"CMakeFiles/client_test.dir/client_test.o" \
"CMakeFiles/client_test.dir/print.o" \
"CMakeFiles/client_test.dir/torrent_view.o" \
"CMakeFiles/client_test.dir/session_view.o"

# External object files for target client_test
client_test_EXTERNAL_OBJECTS =

client_test: CMakeFiles/client_test.dir/client_test.o
client_test: CMakeFiles/client_test.dir/print.o
client_test: CMakeFiles/client_test.dir/torrent_view.o
client_test: CMakeFiles/client_test.dir/session_view.o
client_test: CMakeFiles/client_test.dir/build.make
client_test: CMakeFiles/client_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ysr/project/libtorrent/examples/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable client_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/client_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/client_test.dir/build: client_test

.PHONY : CMakeFiles/client_test.dir/build

CMakeFiles/client_test.dir/requires: CMakeFiles/client_test.dir/client_test.o.requires
CMakeFiles/client_test.dir/requires: CMakeFiles/client_test.dir/print.o.requires
CMakeFiles/client_test.dir/requires: CMakeFiles/client_test.dir/torrent_view.o.requires
CMakeFiles/client_test.dir/requires: CMakeFiles/client_test.dir/session_view.o.requires

.PHONY : CMakeFiles/client_test.dir/requires

CMakeFiles/client_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/client_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/client_test.dir/clean

CMakeFiles/client_test.dir/depend:
	cd /home/ysr/project/libtorrent/examples && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ysr/project/libtorrent/examples /home/ysr/project/libtorrent/examples /home/ysr/project/libtorrent/examples /home/ysr/project/libtorrent/examples /home/ysr/project/libtorrent/examples/CMakeFiles/client_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/client_test.dir/depend

