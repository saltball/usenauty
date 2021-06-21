cmake_minimum_required(VERSION 3.1)
project(usenauty)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_STANDARD 17)

set( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) 

set( LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) 
LINK_DIRECTORIES(LIBRARY_OUTPUT_PATH)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

# 3rdParty library
# nauty
add_subdirectory(libnauty)

# source file
add_subdirectory(src)