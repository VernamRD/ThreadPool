add_library(Worker STATIC
        IThreadWorker.h
        ThreadWorker.h
        ThreadWorker.cpp
)

target_compile_definitions(Worker PRIVATE THREAD_WORKER_SLEEP_TIME_MS=1)

target_include_directories(Worker PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Worker PUBLIC Logger Event Task)