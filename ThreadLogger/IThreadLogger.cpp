#include "IThreadLogger.h"

#include "ThreadLoggerGeneric.h"

IThreadLogger& IThreadLogger::get()
{
    static ThreadLoggerGeneric instance;
    return instance;
}
