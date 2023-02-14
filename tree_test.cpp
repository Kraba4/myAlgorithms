//
// Created by HONOR on 14.02.2023.
//
#include <iostream>
#include "myStructures.h"
#include <random>
#include "Utils.h"
int main(){
    my::TreeAVL<int> tree;
//    tree.insert(1);
//    tree.erase(1);
//    tree.print();
    std::vector a= utils::create_random_vector(1000, 1000, time(NULL));
    auto gen =  std::shuffle_order_engine<std::minstd_rand0, 256>(time(NULL));
    std::shuffle(a.begin(), a.end(), gen);
    for(auto x : a){
        tree.insert(x);
    }
//    tree.print();
    std::vector b = a;

    std::shuffle(b.begin(), b.end(), gen);
    for(auto x : b){
        tree.erase(x);
    }
    tree.print();

}