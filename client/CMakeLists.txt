cmake_minimum_required(VERSION 3.16)
project(client)

set(CMAKE_C_STANDARD 99)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

add_subdirectory(common)

aux_source_directory(./src DIR_SRCS)

include_directories(include common)

add_executable(client ${DIR_SRCS})

target_link_libraries(client common)
