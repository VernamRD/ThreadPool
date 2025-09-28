add_library(Event STATIC
        Event.h
        Event.cpp
)

target_include_directories(Event PUBLIC
${CMAKE_CURRENT_SOURCE_DIR})