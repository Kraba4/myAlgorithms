//
// Created by HONOR on 13.02.2023.
//

#include <algorithm>
#include <execution>
#include <iomanip>
#include "Utils.h"
#include "MyAlgorithms.h"

int main(){
    constexpr int BILLION = 1'000'000'000;
    std::vector v = utils::create_random_vector(50'000'000, 100'000'000, time(NULL));
    std::cout << std::fixed << std::setprecision(9) << std::left;
    auto v5 = v;

    auto [t5, max_t5, min_t5] = utils::runAndCheckTime(1, [&v5]() {
        std::sort(std::execution::par, v5.begin(), v5.end());
    });
    std::cout << "STD par: " << t5 / BILLION << " ms\n";
    auto v2 = v;

    auto [t2, max_t2, min_t2] = utils::runAndCheckTime(1, [&v2]() {
        std::sort(v2.begin(), v2.end());
    });
    std::cout << "STD: " << t2 / BILLION << " ms\n";

    auto v1 = v;
    auto [t, max_t, min_t] = utils::runAndCheckTime(1, [&v1](){
        my::sort_seq(v1.begin(), v1.end());
    });
    std::cout << "My: " << t/BILLION << " ms\n";

    auto v4 = v;
    auto [t4, max_t4, min_t4] = utils::runAndCheckTime(1, [&v4](){
        my::sort(v4.begin(), v4.end());
    });
    std::cout << "My par with thread init: " << t4/BILLION << " ms\n";

    my::ThreadPool pool(4);
    auto v3 = v;
    auto [t3, max_t3, min_t3] = utils::runAndCheckTime(1, [&v3, &pool](){
        my::sort(pool, v3.begin(), v3.end());
    });
    std::cout << "My par: " << t3/BILLION << " ms\n";

    std::cout << "is correct: " << std::equal(v2.begin(), v2.end(), v3.begin(), v3.end()) << std::endl;
    std::cout << "is correct: " << std::equal(v2.begin(), v2.end(), v1.begin(), v1.end()) << std::endl;
    std::cout << "is correct: " << std::equal(v2.begin(), v2.end(), v4.begin(), v4.end()) << std::endl;
//    for(auto x : v){
//        std::cout << x << ' ';
//    }
//    std::cout << std::endl;
//    for(auto x : v3){
//        std::cout << x << ' ';
//    }
}