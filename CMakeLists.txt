cmake_minimum_required(VERSION 3.16)
project(but_multiplier_gen LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(but_core
  but/cli.cpp
  but/generator.cpp
)


target_include_directories(but_core PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_options(but_core PRIVATE
  -Wall -Wextra -Wpedantic
)

add_executable(but_gen
  main.cpp
  tests/generator.cpp
)

target_link_libraries(but_gen PRIVATE but_core)
target_compile_options(but_gen PRIVATE -Wall -Wextra -Wpedantic)
