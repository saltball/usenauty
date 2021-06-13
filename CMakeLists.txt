cmake_minimum_required(VERSION 3.1)
project(usenauty)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

# 3rdParty library
# nauty
add_subdirectory(libnauty)

# source file
add_subdirectory(src)