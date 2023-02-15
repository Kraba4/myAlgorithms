//
// Created by HONOR on 14.02.2023.
//
#include <iostream>
#include "myStructures.h"
#include <random>
#include <set>
#include "Utils.h"
int main(){
    std::set<int> c;
    my::TreeAVL<int> tree;
//    tree.insert(1);
//    tree.erase(1);
//    tree.print();
    std::vector a= utils::create_random_vector(20, 40, time(NULL));
    auto gen =  std::shuffle_order_engine<std::minstd_rand0, 256>(time(NULL));
    std::shuffle(a.begin(), a.end(), gen);
    for(auto x : a){
        std::cout << x<< ' ';
        tree.insert(x);
    }
    std::cout << std::endl;
    tree.print();
    std::cout << std::endl;
    auto it = tree.begin();
    for(int i=0;i<6;i++){
        std::cout << *it << ' ';
        ++it;
    }
    for(int i=0;i<6;i++){
        std::cout << *it << ' ';
        --it;
    }
    std::cout << *it;
    std::cout << std::endl;
    tree.print();
    std::cout << std::endl;
    std::vector b = a;

    std::shuffle(b.begin(), b.end(), gen);

    for(int i=0;i<b.size()/2; ++i){
        std::cout << b[i] << ' ';
        tree.erase(b[i]);
        tree.print();
        std::cout << std::endl;
    }
    std::cout << std::endl;
    tree.print();

}