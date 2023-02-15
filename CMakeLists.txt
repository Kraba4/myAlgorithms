
project(myAlgorithms)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS " -fsanitize=address")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 ")
add_executable(find_test ThreadPool.cpp ThreadPool.h find_test.cpp ModuleVector.cpp ModuleVector.h MyAlgorithms.h Utils.h Utils.cpp BlockingQueue.h AlignedField.h)
add_executable(sort_test sort_test.cpp Utils.h Utils.cpp ThreadPool.cpp ThreadPool.h)
add_executable(tree_test tree_test.cpp myStructures.h Utils.h Utils.cpp)
add_executable(tree_test_zaxar tree_test_zaxar.cpp)