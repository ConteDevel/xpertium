cmake_minimum_required(VERSION 3.5)

project(displays VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(TINYXML2_DIR "${PROJECT_SOURCE_DIR}/third-party/tinyxml2")
set(TEST_DIR "${PROJECT_SOURCE_DIR}/test")
set(LIB_DIR "${PROJECT_SOURCE_DIR}/lib")

add_library(tinyxml2 STATIC "${TINYXML2_DIR}/tinyxml2.cpp")
target_include_directories(tinyxml2 INTERFACE ${TINYXML2_DIR})
add_library(xpertium INTERFACE)
target_include_directories(xpertium INTERFACE ${LIB_DIR})
add_executable(
    ${PROJECT_NAME}
    "${TEST_DIR}/main.cpp"
)
target_link_libraries(${PROJECT_NAME} xpertium tinyxml2)
