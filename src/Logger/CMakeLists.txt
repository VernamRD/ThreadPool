add_library(Logger STATIC
        ILogger.h
        ILogger.cpp
)

target_include_directories(Logger PUBLIC
${CMAKE_CURRENT_SOURCE_DIR})