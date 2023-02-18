//
// Created by HONOR on 18.02.2023.
//
#include <iostream>
#include <tuple>
#include <conio.h>
#include "myStructures.h"
#include <iomanip>
template<typename T>
void print(my::TreeAVL<T>& tree){

    for(int i=0;i<8;++i) {
        std::cout << std::endl;
    }
    int max_dep;
    auto v = tree.collectNodes(max_dep);
    int j=1;
    int last = 0;
    for(int i=1;i<(int)std::pow(2,max_dep+1); ++i){
        if(last!=(int)std::log2(i)){
            std::cout << std::endl;
            last = (int)std::log2(i);
        }
        std::cout.width((int)std::pow(2,(max_dep - (int)std::log2(i))) *6);
        if(std::get<2>(v[j]) == i){
            std::cout << std::get<0>(v[j]);
            ++j;
        }else{
            std::cout << "     ";
        }
        std::cout.width((int)std::pow(2,(max_dep - (int)std::log2(i))) * 6);
        std::cout << ' ';

    }
}
int main(){
    my::TreeAVL<int> tree;
    tree.insert(5);
    tree.insert(6);
    tree.insert(4);
    tree.insert(7);
    tree.insert(3);
    tree.insert(1);
    tree.insert(0);
    tree.insert(-1);

    print(tree);
    while(true){
        int x;
        std::cin >> x;
        tree.insert(x);
        print(tree);
    }
}