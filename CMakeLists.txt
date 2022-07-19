cmake_minimum_required(VERSION 3.10)
project(DataStream VERSION 1.0 LANGUAGES C CXX)

# Set compile options
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_BUILD_TYPE "Release")

# Find packages
find_package(HDF5 COMPONENTS C HL)

# Source files
include_directories(include)
file(GLOB SOURCE "src/*.cc")

# Compile : library for external use
add_library(DataStream SHARED ${SOURCE})
target_include_directories(DataStream PUBLIC ${HDF5_C_INCLUDE_DIRS})
target_link_libraries(DataStream PUBLIC ${HDF5_C_LIBRARIES}
										${HDF5_C_HL_LIBRARIES})

set(${PROJECT_NAME}_INCLUDE ${PROJECT_SOURCE_DIR}/include 
							${HDF5_C_INCLUDE_DIRS}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

# Compile : example programs
add_subdirectory(example)