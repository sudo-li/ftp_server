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
CMAKE_SOURCE_DIR = /home/ftp_server/server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ftp_server/server/bulid

# Include any dependencies generated for this target.
include common/CMakeFiles/common.dir/depend.make

# Include the progress variables for this target.
include common/CMakeFiles/common.dir/progress.make

# Include the compile flags for this target's objects.
include common/CMakeFiles/common.dir/flags.make

common/CMakeFiles/common.dir/common.c.o: common/CMakeFiles/common.dir/flags.make
common/CMakeFiles/common.dir/common.c.o: ../common/common.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ftp_server/server/bulid/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object common/CMakeFiles/common.dir/common.c.o"
	cd /home/ftp_server/server/bulid/common && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/common.dir/common.c.o   -c /home/ftp_server/server/common/common.c

common/CMakeFiles/common.dir/common.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/common.dir/common.c.i"
	cd /home/ftp_server/server/bulid/common && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ftp_server/server/common/common.c > CMakeFiles/common.dir/common.c.i

common/CMakeFiles/common.dir/common.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/common.dir/common.c.s"
	cd /home/ftp_server/server/bulid/common && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ftp_server/server/common/common.c -o CMakeFiles/common.dir/common.c.s

# Object files for target common
common_OBJECTS = \
"CMakeFiles/common.dir/common.c.o"

# External object files for target common
common_EXTERNAL_OBJECTS =

../lib/libcommon.so: common/CMakeFiles/common.dir/common.c.o
../lib/libcommon.so: common/CMakeFiles/common.dir/build.make
../lib/libcommon.so: common/CMakeFiles/common.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ftp_server/server/bulid/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C shared library ../../lib/libcommon.so"
	cd /home/ftp_server/server/bulid/common && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/common.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
common/CMakeFiles/common.dir/build: ../lib/libcommon.so

.PHONY : common/CMakeFiles/common.dir/build

common/CMakeFiles/common.dir/clean:
	cd /home/ftp_server/server/bulid/common && $(CMAKE_COMMAND) -P CMakeFiles/common.dir/cmake_clean.cmake
.PHONY : common/CMakeFiles/common.dir/clean

common/CMakeFiles/common.dir/depend:
	cd /home/ftp_server/server/bulid && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ftp_server/server /home/ftp_server/server/common /home/ftp_server/server/bulid /home/ftp_server/server/bulid/common /home/ftp_server/server/bulid/common/CMakeFiles/common.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : common/CMakeFiles/common.dir/depend

