# ThreadPool
A library for using a thread pool to perform tasks with priorities and dependencies

## Dependence
| C++       |         23 |
|:----------|-----------:|
| Toolchain |       MSVC |
| CMake     |        4.0 |

## Log
* Inherit and implement interface `ILogger` 
* Create `YouLogger : public ILogger` instance and put in `LoggerManager::set_instance()` for connect to you log system
