cmake_minimum_required(VERSION 2.8)

project(materials)
set(CMAKE_CXX_STANDARD 14)
add_executable(
    ${PROJECT_NAME}
    "main.cpp"
    "expert.hpp"
    "expression.hpp"
    "question.hpp"
    "rule.hpp"
)
