//
// Created by HONOR on 07.02.2023.
//

#ifndef MYALGORITHMS_UTILS_H
#define MYALGORITHMS_UTILS_H
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <execution>
#include <functional>

namespace utils {
    struct FindTest {
        size_t size;
        size_t indexToBeFound;
    };
    void testFindAndPrint(std::function<
        std::vector<unsigned>::iterator(std::vector<unsigned>::iterator,std::vector<unsigned>::iterator, unsigned )> find,
        std::initializer_list<FindTest> tests, unsigned seed = 1, unsigned numberOfRuns = 50, long long range = 5'000'000'000);

    std::vector<unsigned int> create_random_vector(size_t size, unsigned int range, unsigned int seed);


    template<typename Function, typename ...Args>
    std::tuple<double, double, double> runAndCheckTime(int numberOfRuns, const Function &fn) {
        std::vector<long long> results;
        long long max_result = 0;
        long long min_result = 999999999999;
        for (int i = 0; i < numberOfRuns; i++) {
            auto start = std::chrono::high_resolution_clock::now();
            fn();
            auto end = std::chrono::high_resolution_clock::now();

            results.push_back((end - start).count());
            max_result = std::max(max_result, (end - start).count());
            min_result = std::min(min_result, (end - start).count());
//            std::cout << results.back() / 1'000'000'000. << " s " << std::endl;
        }
        return std::make_tuple(std::reduce(results.begin(), results.end()) / results.size(),
                               max_result,
                               min_result);
    }
}
#endif //MYALGORITHMS_UTILS_H
