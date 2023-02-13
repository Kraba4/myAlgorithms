//
// Created by HONOR on 13.02.2023.
//

#include <algorithm>
#include <execution>
#include <iomanip>
#include "Utils.h"
int main(){

    constexpr int BILLION = 1'000'000'000;
    auto v = utils::create_random_vector(50'000'000, 10'000'000, time(NULL));
    auto [t, max_t, min_t] = utils::runAndCheckTime(1, [&v](){
        std::sort(v.begin(), v.end());
    });
    std::cout << std::fixed << std::setprecision(9) << std::left;
    std::cout << "Seq: " << t/BILLION << " ms\n";
    auto v2 = utils::create_random_vector(50'000'000, 10'000'000, time(NULL));

    auto [t2, max_t2, min_t2] = utils::runAndCheckTime(1, [&v2](){
        std::sort(std::execution::par, v2.begin(), v2.end());
    });
    std::cout << std::fixed << std::setprecision(9) << std::left;
    std::cout << "Par: " << t2/BILLION << " ms\n";
}