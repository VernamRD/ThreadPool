add_library(Worker STATIC
        IThreadWorker.h
        ThreadWorker.h
        ThreadWorker.cpp
)

target_include_directories(Worker PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Worker PUBLIC Logger Event Task)