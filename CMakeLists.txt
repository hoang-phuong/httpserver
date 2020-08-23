cmake_minimum_required(VERSION 3.0)

# set the project name
project(HttpServer VERSION 1.0)

# set executable name
set(executable "httpserver")

# set C compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
  -O2 \
  -std=c99 \
  -Wall \
  -Wextra \
  -Wfloat-equal \
  -Wundef \
  -Wshadow \
  -Wpointer-arith \
  -Wcast-align \
  -Wstrict-prototypes \
  -Wcast-qual \
  -Wswitch-default \
  -Wswitch-enum \
  -Wconversion \
  -Wunreachable-code \
  -pipe \
  -fexceptions \
  -fstack-protector \
")

set(CMAKE_BINARY_DIR "out")

# glob source files
file(GLOB_RECURSE sources "src/*.c")

# add include directory
include_directories(${CMAKE_SOURCE_DIR})

# add the executable
add_executable(${executable} ${sources})

# set build ooutput directories
set_target_properties(${executable}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)