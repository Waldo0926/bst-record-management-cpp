cmake_minimum_required(VERSION 3.16)
project(bst_record_management_cpp VERSION 2.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(bst_core
    src/BinarySearchTree.cpp
    src/Storage.cpp
)

target_include_directories(bst_core PUBLIC include)

if(MSVC)
    target_compile_options(bst_core PRIVATE /W4)
else()
    target_compile_options(bst_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(bst_record_manager src/main.cpp)
target_link_libraries(bst_record_manager PRIVATE bst_core)

if(MSVC)
    target_compile_options(bst_record_manager PRIVATE /W4)
else()
    target_compile_options(bst_record_manager PRIVATE -Wall -Wextra -Wpedantic)
endif()

enable_testing()
add_executable(bst_tests tests/bst_tests.cpp)
target_link_libraries(bst_tests PRIVATE bst_core)

if(MSVC)
    target_compile_options(bst_tests PRIVATE /W4)
else()
    target_compile_options(bst_tests PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_test(NAME bst_tests COMMAND bst_tests)
