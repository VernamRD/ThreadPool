project(Google_tests)
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# adding the Google_Tests_run target
add_executable(Google_Tests_run
        ../src/Event/Tests.cpp
        ../src/Logger/Tests.cpp
        ../src/Worker/Tests.cpp
        ../src/Task/Tests.cpp)

target_link_libraries(Google_Tests_run gtest gtest_main Event Logger Worker Task)