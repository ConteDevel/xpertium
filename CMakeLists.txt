cmake_minimum_required(VERSION 3.5)

project(materials VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)
set(TEST_DIR "${PROJECT_SOURCE_DIR}/test")
set(LIB_DIR "${PROJECT_SOURCE_DIR}/lib")

add_library(xpertium INTERFACE)
target_include_directories(xpertium INTERFACE ${LIB_DIR})
add_executable(
    ${PROJECT_NAME}
    "${TEST_DIR}/kb.cpp"
    "${TEST_DIR}/main.cpp"
)
target_link_libraries(${PROJECT_NAME} xpertium tinyxml)
