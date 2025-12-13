cmake_minimum_required(VERSION 3.10.0)
project(test2 VERSION 0.1.0 LANGUAGES C CXX)

add_executable(test2 
    src/main.cpp 
    src/sum.cpp)

target_include_directories(test2 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include)
include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
