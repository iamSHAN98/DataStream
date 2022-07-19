# cmake_minimum_required(VERSION 3.10)
# project(Example LANGUAGES C CXX)

# # DataStream Source Path
# # Command Line Override : -DDSPATH=path/to/source
# set(DSPATH ..)
# add_subdirectory(${DSPATH} DataStream)

add_executable(Write write.cc)
target_include_directories(Write PRIVATE ${DataStream_INCLUDE})
target_link_libraries(Write PRIVATE DataStream)

add_executable(Read read.cc)
target_include_directories(Read PRIVATE ${DataStream_INCLUDE})
target_link_libraries(Read PRIVATE DataStream)