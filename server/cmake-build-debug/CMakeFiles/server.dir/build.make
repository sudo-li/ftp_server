# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_COMMAND = /home/sudo-li/CLion-2021.2/clion-2021.2/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/sudo-li/CLion-2021.2/clion-2021.2/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/sudo-li/CLionProjects/ftp_server/server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sudo-li/CLionProjects/ftp_server/server/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/server.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/server.dir/flags.make

CMakeFiles/server.dir/src/ls.c.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/src/ls.c.o: ../src/ls.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sudo-li/CLionProjects/ftp_server/server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/server.dir/src/ls.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/server.dir/src/ls.c.o -c /home/sudo-li/CLionProjects/ftp_server/server/src/ls.c

CMakeFiles/server.dir/src/ls.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server.dir/src/ls.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sudo-li/CLionProjects/ftp_server/server/src/ls.c > CMakeFiles/server.dir/src/ls.c.i

CMakeFiles/server.dir/src/ls.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server.dir/src/ls.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sudo-li/CLionProjects/ftp_server/server/src/ls.c -o CMakeFiles/server.dir/src/ls.c.s

CMakeFiles/server.dir/src/server.c.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/src/server.c.o: ../src/server.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sudo-li/CLionProjects/ftp_server/server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/server.dir/src/server.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/server.dir/src/server.c.o -c /home/sudo-li/CLionProjects/ftp_server/server/src/server.c

CMakeFiles/server.dir/src/server.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server.dir/src/server.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sudo-li/CLionProjects/ftp_server/server/src/server.c > CMakeFiles/server.dir/src/server.c.i

CMakeFiles/server.dir/src/server.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server.dir/src/server.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sudo-li/CLionProjects/ftp_server/server/src/server.c -o CMakeFiles/server.dir/src/server.c.s

# Object files for target server
server_OBJECTS = \
"CMakeFiles/server.dir/src/ls.c.o" \
"CMakeFiles/server.dir/src/server.c.o"

# External object files for target server
server_EXTERNAL_OBJECTS =

../bin/server: CMakeFiles/server.dir/src/ls.c.o
../bin/server: CMakeFiles/server.dir/src/server.c.o
../bin/server: CMakeFiles/server.dir/build.make
../bin/server: ../lib/libcommon.so
../bin/server: CMakeFiles/server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/sudo-li/CLionProjects/ftp_server/server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable ../bin/server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/server.dir/build: ../bin/server
.PHONY : CMakeFiles/server.dir/build

CMakeFiles/server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/server.dir/clean

CMakeFiles/server.dir/depend:
	cd /home/sudo-li/CLionProjects/ftp_server/server/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sudo-li/CLionProjects/ftp_server/server /home/sudo-li/CLionProjects/ftp_server/server /home/sudo-li/CLionProjects/ftp_server/server/cmake-build-debug /home/sudo-li/CLionProjects/ftp_server/server/cmake-build-debug /home/sudo-li/CLionProjects/ftp_server/server/cmake-build-debug/CMakeFiles/server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/server.dir/depend

