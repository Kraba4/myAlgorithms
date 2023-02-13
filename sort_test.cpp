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
    std::vector v = utils::create_random_vector(10'000'000, 10'000'000, time(NULL));
    auto v2 = v;

    auto [t2, max_t2, min_t2] = utils::runAndCheckTime(1, [&v2]() {
        std::sort(v2.begin(), v2.end());
    });
    std::cout << std::fixed << std::setprecision(9) << std::left;
    std::cout << "STD: " << t2 / BILLION << " ms\n";

    auto v1 = v;
    auto [t, max_t, min_t] = utils::runAndCheckTime(1, [&v1](){
        my::sort(v1.begin(), v1.end());
    });
    std::cout << "My: " << t/BILLION << " ms\n";

    std::cout << "is correct: " << std::equal(v2.begin(), v2.end(), v1.begin(), v1.end()) << std::endl;
}