add_library(Pool STATIC
        ThreadPoolTypes.h
        ThreadPool.h
        ThreadPool.cpp
)

target_include_directories(Pool PUBLIC
${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Pool PUBLIC Logger Event Task Worker)