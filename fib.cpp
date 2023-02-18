//
// Created by HONOR on 18.02.2023.
//

#include <iostream>
#include <vector>
#include <iomanip>

int main(){
    std::vector<int> fib;
    std::vector<int> fib_sum;
    fib.push_back(1);
    fib.push_back(2);
    fib_sum.push_back(1);
    fib_sum.push_back(3);
    for(int i=2;i<64;i++){
        fib.push_back(fib[i-1] + fib[i-2]);
        fib_sum.push_back(fib_sum[i-1] + fib[i]);
    }

    for(int i=1; i < 64;i++){
        std::cout << std::setw(10) << i << ' ';
    }
    std::cout << std::endl;
    for(auto x : fib){
        std::cout  << std::setw(10)<< x << ' ';
    }
    std::cout << std::endl;
    for(auto x : fib_sum){
        std::cout  << std::setw(10)<< x << ' ';
    }
    std::cout << std::endl;
    std::vector<int> fib_sum_sum;
    fib_sum_sum.push_back(0);
    for(int i=1;i<64;i++){
        fib_sum_sum.push_back(fib_sum_sum[i-1] + fib_sum[i]);
    }
    std::cout << "sum sum\n";
    for(auto x : fib_sum_sum){
        std::cout  << std::setw(10)<< x << ' ';
    }
    std::cout << std::endl;
    for(int i=1; i<64; i++){
        std::cout  << std::setw(10)<< (int)std::pow(2,i) << ' ';
    }
    std::cout << std::endl;

    for(int i=1; i<64; i++){
        std::cout  << std::setw(10)<< fib_sum_sum[i-1]/std::pow(2,i) << ' ';
    }
    std::cout << std::endl;

}