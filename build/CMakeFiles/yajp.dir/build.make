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
CMAKE_SOURCE_DIR = /home/wangao/projects/yajp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/wangao/projects/yajp/build

# Include any dependencies generated for this target.
include CMakeFiles/yajp.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/yajp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/yajp.dir/flags.make

CMakeFiles/yajp.dir/main.cpp.o: CMakeFiles/yajp.dir/flags.make
CMakeFiles/yajp.dir/main.cpp.o: CMakeFiles/yajp.dir/includes_CXX.rsp
CMakeFiles/yajp.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wangao/projects/yajp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/yajp.dir/main.cpp.o"
	/home/wangao/projects/emsdk/upstream/emscripten/em++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/yajp.dir/main.cpp.o -c /home/wangao/projects/yajp/main.cpp

CMakeFiles/yajp.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/yajp.dir/main.cpp.i"
	/home/wangao/projects/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wangao/projects/yajp/main.cpp > CMakeFiles/yajp.dir/main.cpp.i

CMakeFiles/yajp.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/yajp.dir/main.cpp.s"
	/home/wangao/projects/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wangao/projects/yajp/main.cpp -o CMakeFiles/yajp.dir/main.cpp.s

CMakeFiles/yajp.dir/parser.cpp.o: CMakeFiles/yajp.dir/flags.make
CMakeFiles/yajp.dir/parser.cpp.o: CMakeFiles/yajp.dir/includes_CXX.rsp
CMakeFiles/yajp.dir/parser.cpp.o: ../parser.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wangao/projects/yajp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/yajp.dir/parser.cpp.o"
	/home/wangao/projects/emsdk/upstream/emscripten/em++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/yajp.dir/parser.cpp.o -c /home/wangao/projects/yajp/parser.cpp

CMakeFiles/yajp.dir/parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/yajp.dir/parser.cpp.i"
	/home/wangao/projects/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wangao/projects/yajp/parser.cpp > CMakeFiles/yajp.dir/parser.cpp.i

CMakeFiles/yajp.dir/parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/yajp.dir/parser.cpp.s"
	/home/wangao/projects/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wangao/projects/yajp/parser.cpp -o CMakeFiles/yajp.dir/parser.cpp.s

CMakeFiles/yajp.dir/lexer.cpp.o: CMakeFiles/yajp.dir/flags.make
CMakeFiles/yajp.dir/lexer.cpp.o: CMakeFiles/yajp.dir/includes_CXX.rsp
CMakeFiles/yajp.dir/lexer.cpp.o: ../lexer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wangao/projects/yajp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/yajp.dir/lexer.cpp.o"
	/home/wangao/projects/emsdk/upstream/emscripten/em++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/yajp.dir/lexer.cpp.o -c /home/wangao/projects/yajp/lexer.cpp

CMakeFiles/yajp.dir/lexer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/yajp.dir/lexer.cpp.i"
	/home/wangao/projects/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wangao/projects/yajp/lexer.cpp > CMakeFiles/yajp.dir/lexer.cpp.i

CMakeFiles/yajp.dir/lexer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/yajp.dir/lexer.cpp.s"
	/home/wangao/projects/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wangao/projects/yajp/lexer.cpp -o CMakeFiles/yajp.dir/lexer.cpp.s

CMakeFiles/yajp.dir/error.cpp.o: CMakeFiles/yajp.dir/flags.make
CMakeFiles/yajp.dir/error.cpp.o: CMakeFiles/yajp.dir/includes_CXX.rsp
CMakeFiles/yajp.dir/error.cpp.o: ../error.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wangao/projects/yajp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/yajp.dir/error.cpp.o"
	/home/wangao/projects/emsdk/upstream/emscripten/em++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/yajp.dir/error.cpp.o -c /home/wangao/projects/yajp/error.cpp

CMakeFiles/yajp.dir/error.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/yajp.dir/error.cpp.i"
	/home/wangao/projects/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wangao/projects/yajp/error.cpp > CMakeFiles/yajp.dir/error.cpp.i

CMakeFiles/yajp.dir/error.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/yajp.dir/error.cpp.s"
	/home/wangao/projects/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wangao/projects/yajp/error.cpp -o CMakeFiles/yajp.dir/error.cpp.s

# Object files for target yajp
yajp_OBJECTS = \
"CMakeFiles/yajp.dir/main.cpp.o" \
"CMakeFiles/yajp.dir/parser.cpp.o" \
"CMakeFiles/yajp.dir/lexer.cpp.o" \
"CMakeFiles/yajp.dir/error.cpp.o"

# External object files for target yajp
yajp_EXTERNAL_OBJECTS =

../public/yajp.js: CMakeFiles/yajp.dir/main.cpp.o
../public/yajp.js: CMakeFiles/yajp.dir/parser.cpp.o
../public/yajp.js: CMakeFiles/yajp.dir/lexer.cpp.o
../public/yajp.js: CMakeFiles/yajp.dir/error.cpp.o
../public/yajp.js: CMakeFiles/yajp.dir/build.make
../public/yajp.js: CMakeFiles/yajp.dir/objects1.rsp
../public/yajp.js: CMakeFiles/yajp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/wangao/projects/yajp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable ../public/yajp.js"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/yajp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/yajp.dir/build: ../public/yajp.js

.PHONY : CMakeFiles/yajp.dir/build

CMakeFiles/yajp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/yajp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/yajp.dir/clean

CMakeFiles/yajp.dir/depend:
	cd /home/wangao/projects/yajp/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wangao/projects/yajp /home/wangao/projects/yajp /home/wangao/projects/yajp/build /home/wangao/projects/yajp/build /home/wangao/projects/yajp/build/CMakeFiles/yajp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/yajp.dir/depend

