cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project ("RedBlackTree")

include_directories(src)

add_library(RBTree STATIC src/RedBlackTree.cpp src/Allocator.cpp src/Operations.cpp)

add_subdirectory(lib/Catch2)

add_executable(Tests)
target_sources(Tests PRIVATE tests/RBT.cpp)
target_link_libraries(Tests PUBLIC RBTree Catch2::Catch2WithMain)
