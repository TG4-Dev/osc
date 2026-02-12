cmake_minimum_required(VERSION 4.0)
project(osc)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compile commads for lsp
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/FetchDependencies.cmake)
find_package(Vulkan REQUIRED)

add_subdirectory(test)
add_subdirectory(src)
