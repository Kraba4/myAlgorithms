//
// Created by HONOR on 05.02.2023.
//
#include <iostream>
#include <random>
#include <execution>

#include "ThreadPool.h"
#include "FindPool.h"
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
    const unsigned int SEED =  time(NULL);

    utils::testFindAndPrint(stdFindSeq, {{5000, 200}, {500'000, 200}, {50'000'000}}, SEED);
    utils::testFindAndPrint(stdFindPar, {{5000, 200}, {500'000, 200}, {50'000'000}}, SEED);


    my::ThreadPool pool(4);
    auto myFind = [&pool](std::vector<unsigned>::iterator begin,
                          std::vector<unsigned>::iterator end, unsigned val){
        return my::find(pool, begin, end, val);
    };
    utils::testFindAndPrint(myFind, {{5000, 200}, {500'000, 200}, {50'000'000}}, SEED);

}