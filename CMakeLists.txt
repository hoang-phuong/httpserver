cmake_minimum_required(VERSION 3.10)

# set the project name
project(HttpServer VERSION 1.0)

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

# glob source files
file(GLOB_RECURSE sources RELATIVE "src" "*.c")

# add include directory
include_directories(${CMAKE_SOURCE_DIR})

# add the executable
add_executable(httpserver ${source})