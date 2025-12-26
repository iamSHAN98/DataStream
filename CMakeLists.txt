cmake_minimum_required(VERSION 3.10)
project(DataStream VERSION 1.0 LANGUAGES C CXX)

# Set Compile Options
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Find Packages
find_package(HDF5 COMPONENTS C HL)

# Source Files
include_directories(include)
file(GLOB SOURCE "src/*.cc")

# Compile : Library
add_library(DataStream SHARED ${SOURCE})
target_include_directories(DataStream PUBLIC ${HDF5_C_INCLUDE_DIRS})
target_link_libraries(DataStream PUBLIC ${HDF5_C_LIBRARIES} ${HDF5_C_HL_LIBRARIES})

set(${PROJECT_NAME}Include ${PROJECT_SOURCE_DIR}/include ${HDF5_C_INCLUDE_DIRS}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

# Compile : Example Programs
add_subdirectory(example)
