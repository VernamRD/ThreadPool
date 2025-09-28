#pragma once

#include <map>
#include <shared_mutex>
#include "Task.h"

class TaskPipe
{
public:

private:
    std::shared_mutex mutex;
    std::map<threadpool::ETaskPriority, std::vector<threadpool::Task>> tasks;
};