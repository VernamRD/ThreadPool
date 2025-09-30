# ThreadPool

A library for using a thread pool to perform tasks with priorities and dependencies

## Dependence

| Tool  |      |
|:------|-----:|
| C++   |   23 |
| CMake | 4.0+ |

| Compiler |   Version |
|:---------|----------:|
| MSVC     | undefined |
| Clang    |        19 |
| GCC      |        13 |

## Log

* Inherit and implement interface `ILogger`
* Create `YouLogger : public ILogger` instance and put in `LoggerManager::set_instance()` for connect to you log system
