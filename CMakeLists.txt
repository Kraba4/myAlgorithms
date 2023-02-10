
project(myAlgorithms)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS " -fsanitize=address")
add_executable(myAlgorithms ThreadPool.cpp ThreadPool.h main.cpp ModuleVector.cpp ModuleVector.h MyAlgorithms.h Utils.h Utils.cpp BlockingQueue.h AlignedField.h)
