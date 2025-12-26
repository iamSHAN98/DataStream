# cmake_minimum_required(VERSION 3.10)
# project(Example LANGUAGES C CXX)

# DataStream Source Path
# Command Line Override : -DDataStreamPath=path/to/DataStream
# set(DataStreamPath ..)
# add_subdirectory(${DataStreamPath} DataStream)

add_executable(Write write.cc)
target_include_directories(Write PRIVATE ${DataStreamInclude})
target_link_libraries(Write PRIVATE DataStream)

add_executable(Read read.cc)
target_include_directories(Read PRIVATE ${DataStreamInclude})
target_link_libraries(Read PRIVATE DataStream)