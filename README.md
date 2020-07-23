# Allocator exploration

This is a toy program that explores how different allocations patterns affect performance.  The demo renders a bunch of cubes organized in a tree that rotate around.  Different allocation patterns are used for the transform memory to explore how this affects performance.


Only tested on Windows with CMake version 3.16.5.  
* Requires OpenGL to be installed and discoverable by CMake
* Requests C++17, but should work on C++14 