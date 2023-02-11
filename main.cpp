//
// Created by HONOR on 05.02.2023.
//
#include <iostream>
#include <random>
#include <execution>

#include "ThreadPool.h"
#include "MyAlgorithms.h"
#include "Utils.h"

auto stdFindSeq = [](std::vector<unsigned>::iterator begin,
                     std::vector<unsigned>::iterator end, unsigned val){
    return std::find(begin, end, val);
};
auto stdFindPar = [](std::vector<unsigned>::iterator begin,
                     std::vector<unsigned>::iterator end, unsigned val){
    return std::find(std::execution::par, begin, end, val);
};

int main(){
    using namespace std::chrono_literals;
    const unsigned int SEED =  time(NULL);
    const unsigned int RANGE = 5'000'000'000;
    //{50'000'000,10'000'000}
#define initTest {{5000, 200}, {500'000, 12'500},{50'000'000,200'000}}
    std::cout << "Seq: \n";
    utils::testFindAndPrint(stdFindSeq, initTest, SEED);
    std::cout << "Par: \n";
//    std::this_thread::sleep_for(4000ms);
    utils::testFindAndPrint(stdFindPar, initTest, SEED);


    std::cout << "My: \n";
//    std::this_thread::sleep_for(1000ms);
    my::ThreadPool pool(4);
//    std::cout << sizeof(pool);
    auto myFind = [&pool](std::vector<unsigned>::iterator begin,
                          std::vector<unsigned>::iterator end, unsigned val){
        return my::find(pool, begin, end, val);
    };

   utils::testFindAndPrint(myFind, initTest , SEED);
//    utils::testFindAndPrint(myFind, { {50'000'000, 100}}, SEED);

//   auto h1 = pool.doAsync([](){std::cout << "hello from 1\n"; return 123;});
//   auto h2 = pool.doAsync([](){
//       std::this_thread::sleep_for(500ms);
//       std::cout << "hello from 2\n";
//   });
//   pool.doAsync([](){
//        std::cout << "hello from 3\n";
//    });
//    pool.doAsync([](){
//        std::cout << "hello from 4\n";
//    });
//    pool.doAsync([](){
//        std::cout << "hello from 5\n";
//    });
//    pool.doAsync([](){
//        std::cout << "hello from 6\n";
//    });
//   h1.wait();
//
//   std::cout << h1.get() << "\n";
//   h2.wait();
}