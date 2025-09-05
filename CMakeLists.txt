cmake_minimum_required(VERSION 3.16)
project(flat_earth_sim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output to build/bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find Python (for matplotlibcpp.h)
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
# NumPy headers are accessed via Python include dirs for matplotlibcpp.h usage.

add_executable(flat_earth_sim
  main_program.cpp
  flat_earth_eom.cpp
  numerical_integration_methods.cpp
  ussa1976.cpp
  spheres.cpp
)

target_include_directories(flat_earth_sim PRIVATE
  ${Python3_INCLUDE_DIRS}
  ${Python3_NumPy_INCLUDE_DIRS}  # may be empty on some systems; OK
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(flat_earth_sim PRIVATE
  Python3::Python
)
