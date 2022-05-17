cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project ("RedBlackTree")

add_subdirectory(lib/Catch2)
add_subdirectory(lib/spdlog)

include_directories(src)

add_library(logger STATIC src/Logger.cpp)
target_link_libraries(logger PUBLIC spdlog::spdlog)

add_library(RBTree STATIC src/Operations.cpp src/Allocator.cpp)
target_link_libraries(RBTree PUBLIC logger)


add_executable(Tests)
target_sources(Tests PRIVATE tests/RBT.cpp)
target_link_libraries(Tests PUBLIC RBTree Catch2::Catch2WithMain spdlog::spdlog)


set(DOCUMENTATION_OUTPUT_PATH "${CMAKE_BINARY_DIR}/docs" CACHE FILEPATH "Specifies where to generate the documentation")
add_subdirectory(docs)