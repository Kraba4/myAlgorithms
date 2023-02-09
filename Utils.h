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
        std::initializer_list<FindTest> tests, unsigned seed = 1, unsigned numberOfRuns = 50, unsigned range = 5'000'000'000);

    std::vector<unsigned int> create_random_vector(size_t size, unsigned int range, unsigned int seed);


    template<typename Function, typename ...Args>
    double runAndCheckTime(int numberOfRuns, const Function &fn) {
        std::vector<long long> results;
        for (int i = 0; i < numberOfRuns; i++) {
            auto start = std::chrono::high_resolution_clock::now();
            fn();
            auto end = std::chrono::high_resolution_clock::now();
            results.push_back((end - start).count());
//            std::cout << results.back() / 1'000'000'000. << " s " << std::endl;
        }
        return std::reduce(results.begin(), results.end()) / results.size();
    }
}
#endif //MYALGORITHMS_UTILS_H
