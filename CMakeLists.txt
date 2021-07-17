cmake_minimum_required(VERSION 3.12)
project(urlcmd VERSION 0.1.0)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)

set(Boost_ARCHITECTURE "-x64")
find_package(Boost COMPONENTS regex program_options REQUIRED)
if(${Boost_FOUND})
    message("Found boost and required packages.")
else()
    message("Could not find boost or boost binaries have not been built.")
endif()
message("Boost Include Directories: ${Boost_INCLUDE_DIR}")

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${Boost_INCLUDE_DIR})

add_subdirectory(src)
