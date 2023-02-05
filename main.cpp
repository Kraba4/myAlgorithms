//
// Created by HONOR on 05.02.2023.
//

#include "ThreadPool.h"
#include "FindPool.h"
#include <iostream>
#include <random>
#include <execution>

using std::cout;

std::vector<unsigned int>  create_random_vector(size_t size, unsigned int range, unsigned int seed){
    std::vector<unsigned int> v;

    std::minstd_rand rnd(seed);

    for(int i=0;i<size;++i){
        v.push_back(rnd() % range);
    }

    return std::move(v);
}
template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v){
    out << &v << "=[ ";
    for(auto& elem : v) {
        out << elem << ' ';
    }
    out << "] \n";
    return out;
}
template<typename Function, typename ...Args>
double callAndCheckTime(int n, const Function& fn){
    std::vector<int> results;
    for(int i=0;i<n;i++) {
        auto start = std::chrono::high_resolution_clock::now();
        fn();
        auto end = std::chrono::high_resolution_clock::now();
        results.push_back((end - start).count());
    }
    return std::reduce(results.begin(), results.end()) / results.size();
}


int main(){
    my::ThreadPool pool(4);

//    my::ThreadPool::Task task([](){
//        std::cout << "hello\n";
//        return 999;
//    });
//    my::ThreadPool::Task task2([](){
//        std::cout << "hello\n";
//        return 1;
//    });
//    my::ThreadPool::Task task3([](){
//        std::cout << "hello\n";
//        return 1;
//    });
//    my::ThreadPool::Task task4([](){
//        std::cout << "hello\n";
//        return 1;
//    });
//    pool.doAsync(task);
//    pool.doAsync(task2);
//    pool.doAsync(task3);
//    pool.doAsync(task4);
//    std::cout << pool.getNumberOfThreads() << std::endl;
//    task.wait();
//    task2.wait();
//    task3.wait();
//    task4.wait();
//    std::cout << any_cast<int>(task.result);

    std::cout << std::fixed << std::setprecision(9) << std::left;
    const unsigned int SEED =  time(NULL);
    const unsigned int RANGE = 1000'000'000;
    constexpr int N_CHECKS = 1;
    constexpr int BILLION = 1'000'000'000;


    for(int size=5000; size<= 5 * std::pow(10, 7); size*=100) {
        auto arr = create_random_vector(size, RANGE, SEED + size);
        std::minstd_rand rnd(SEED + size);
        int valueToFind = arr[arr.size()-1];
//        int valueToFind = arr[100];
//        int valueToFind = arr[rnd() % size];
        decltype(arr.begin()) result;
        double time = callAndCheckTime(N_CHECKS, [&](){
            result = my::find(pool, arr.begin(), arr.end(), valueToFind);
//            result = std::find(std::execution::par, arr.begin(), arr.end(), valueToFind);
        });
        cout << "size = "<< std::setw(9) << size << " time: " << time / BILLION << " s |***| ";// << "--> time2: "<< time2 << " s  <--  \n";
        cout << std::endl;
//        cout << "index = "<< result - arr.begin() << " (" << *result << "==" << valueToFind << ") \n";
    }


}