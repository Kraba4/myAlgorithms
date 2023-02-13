
project(myAlgorithms)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS " -fsanitize=address")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 ")
add_executable(findTest ThreadPool.cpp ThreadPool.h main.cpp ModuleVector.cpp ModuleVector.h MyAlgorithms.h Utils.h Utils.cpp BlockingQueue.h AlignedField.h)
add_executable(sortTest sort_test.cpp Utils.h Utils.cpp)