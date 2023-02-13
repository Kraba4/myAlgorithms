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
auto stdAsync = [](std::vector<unsigned>::iterator begin,
                     std::vector<unsigned>::iterator end, unsigned val){
    return my::find(begin, end, val);
};

int main(){
    using namespace std::chrono_literals;
    const unsigned int SEED =  time(NULL);
//    const unsigned int RANGE = 5'000'000'000;
#define initTest {{5000, SEED%5000}, {500'000, SEED%500'000},{50'000'000,SEED%50'000'000}}
//#define initTest {{5000, 4'900}, {500'000, 499'000},{50'000'000,1'000'000}}
    std::cout << "Seq: \n";
    utils::testFindAndPrint(stdFindSeq, initTest, SEED);
    std::cout << "Par: \n";
    utils::testFindAndPrint(stdFindPar, initTest, SEED);


    std::cout << "My: \n";
    my::ThreadPool pool(4);
    auto myFind = [&pool](std::vector<unsigned>::iterator begin,
                          std::vector<unsigned>::iterator end, unsigned val){
        return my::find(pool, begin, end, val);
    };
    int a[] = {1,2,3};
//   std::sort(std::execution::par, a, a+3);

   utils::testFindAndPrint(myFind, initTest , SEED);

    std::cout << "My std::async : \n";
    utils::testFindAndPrint(stdAsync, initTest , SEED);
//   std::cout << "Async: \n";
//    utils::testFindAndPrint(stdAsync, initTest , SEED);
}