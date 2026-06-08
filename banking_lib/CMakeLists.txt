cmake_minimum_required(VERSION 3.14)
project(banking_lib)

set(CMAKE_CXX_STANDARD 17)

add_library(banking STATIC
    src/Account.cpp
    src/Transaction.cpp
)

target_include_directories(banking PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

option(BUILD_BANKING_TESTS "Build banking tests" OFF)

if(BUILD_BANKING_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    
    file(GLOB TEST_SOURCES "tests/*.cpp")
    add_executable(banking_test ${TEST_SOURCES})
    target_link_libraries(banking_test banking gtest_main gmock)
    add_test(NAME banking_test COMMAND banking_test)
endif()
