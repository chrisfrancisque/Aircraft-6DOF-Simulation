cmake_minimum_required(VERSION 3.16)
project(flat_earth_sim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output to build/bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find Python (for matplotlibcpp.h)
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# Find NumPy include directory
execute_process(
    COMMAND ${Python3_EXECUTABLE} -c "import numpy; print(numpy.get_include())"
    OUTPUT_VARIABLE NUMPY_INCLUDE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    RESULT_VARIABLE NUMPY_RESULT
)

if(NOT NUMPY_RESULT EQUAL 0)
    message(FATAL_ERROR "NumPy not found. Please install it: pip3 install numpy")
endif()

message(STATUS "NumPy include directory: ${NUMPY_INCLUDE_DIR}")

add_executable(flat_earth_sim
    main_program.cpp
    flat_earth_eom.cpp
    numerical_integration_methods.cpp
    ussa1976.cpp
    spheres.cpp
)

target_include_directories(flat_earth_sim PRIVATE
    ${Python3_INCLUDE_DIRS}
    ${NUMPY_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(flat_earth_sim PRIVATE
    Python3::Python
)