cmake_minimum_required(VERSION 3.1)

include_directories(${PROJECT_SOURCE_DIR}/include)

AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} source)

add_executable(cagesearch ${source})
# set( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) 

target_link_libraries(cagesearch nauty)