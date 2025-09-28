cmake_minimum_required(VERSION 4.0)

project(ThreadPool)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Google_tests)

set(SOURCES
        src/main.cpp
)

add_executable(ThreadPool ${SOURCES})

# Logger
add_subdirectory(${PROJECT_SOURCE_DIR}/src/Logger)
target_link_libraries(ThreadPool PRIVATE Logger)

# Event
add_subdirectory(${PROJECT_SOURCE_DIR}/src/Event)
target_link_libraries(ThreadPool PRIVATE Event)

#Task
add_subdirectory(${PROJECT_SOURCE_DIR}/src/Task)
target_link_libraries(ThreadPool PRIVATE Task)

#Worker
add_subdirectory(${PROJECT_SOURCE_DIR}/src/Worker)
target_link_libraries(ThreadPool PRIVATE Worker)

#Pool
add_subdirectory(${PROJECT_SOURCE_DIR}/src/Pool)
target_link_libraries(ThreadPool PRIVATE Pool)