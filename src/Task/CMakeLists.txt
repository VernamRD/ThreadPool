add_library(Task STATIC
        Task.h
        Task.cpp
        TaskPipe.h
        TaskPipe.cpp
)

target_include_directories(Task PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR})