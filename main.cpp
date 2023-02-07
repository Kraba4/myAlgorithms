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
    utils::testFindAndPrint(stdFindSeq, {{5000, 200}, {500'000, 200}, {50'000'000,10'999'000}}, SEED);
    utils::testFindAndPrint(stdFindPar, {{5000, 200}, {500'000, 200}, {50'000'000,100}}, SEED);


    my::ThreadPool pool(4);
    auto myFind = [&pool](std::vector<unsigned>::iterator begin,
                          std::vector<unsigned>::iterator end, unsigned val){
        return my::find(pool, begin, end, val);
    };
   utils::testFindAndPrint(myFind, {{5000, 200}, {500'000, 200}, {50'000'000, 100}}, SEED);

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